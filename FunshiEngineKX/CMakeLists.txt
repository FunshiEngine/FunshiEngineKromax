cmake_minimum_required(VERSION 3.15)

# Versión del producto, fuente ÚNICA: alimenta la de project() y el recurso
# VERSIONINFO del .exe de Windows (packaging/icon/app.rc.in). Es una variable de
# caché para que se pueda sobreescribir al configurar (-DFUNSHI_VERSION=1.2.3),
# que es lo que hace el workflow al publicar: así el número del .exe y el del
# instalador salen del mismo tag, en vez de dos números escritos a mano que
# nadie sincroniza.
set(FUNSHI_VERSION "0.5.6" CACHE STRING "Versión del producto (X.Y.Z)")
# C y CXX: el proyecto es C++, pero las distribuciones de CEF (cef_variables.cmake)
# corren CHECK_C_COMPILER_FLAG en configure y CMake exige el lenguaje habilitado.
project(FunshiEngineC VERSION ${FUNSHI_VERSION} LANGUAGES C CXX)
message(STATUS "FunshiEngineC ${PROJECT_VERSION} (FUNSHI_VERSION=${FUNSHI_VERSION})")
# Sustituciones de configure_file para el .rc: el icono se referencia relativo
# al .rc (se copia al build), la version se lleva a comas (1,2,3,0).
set(FUNSHI_APP_ICON "app.ico")
string(REPLACE "." "," FUNSHI_VERSION_COMMA "${FUNSHI_VERSION}")
set(FUNSHI_VERSION_COMMA "${FUNSHI_VERSION_COMMA},0")

# Configuración básica
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Runtime de C++ de MSVC (CMP0091): el engine y las .dll de script que compila
# en runtime tienen que usar el MISMO CRT. Los SerializeField cruzan la frontera
# de la .dll con std::string, std::vector y std::function: la memoria se aloca
# en un modulo y se libera en el otro, y cada CRT estatico (/MT) trae su propio
# heap (liberar memoria del otro corrompe el heap y el motor muere). Se declara
# explicito para que el contrato sea visible en el build y no un default
# implicito de CMake. En Debug el runtime es /MDd, y FUNSHI_CXX_RUNTIME_FLAG
# hornea ese mismo valor para la linea de compilacion del script.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
set(FUNSHI_CXX_RUNTIME_FLAG "$<IF:$<CONFIG:Debug>,/MDd,/MD>")

# Configuración de rutas. CMAKE_CURRENT_SOURCE_DIR, no CMAKE_SOURCE_DIR: la
# raiz del engine se consumio tambien via add_subdirectory (el exportador de
# juegos), y ahi CMAKE_SOURCE_DIR es el proyecto temporal, no este directorio.
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Búsqueda de archivos fuente
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/*.cpp ${SRC_DIR}/*.c)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${SRC_DIR}/*.h ${SRC_DIR}/*.hpp)

# Las herramientas de proyecto y diagnóstico se comparten entre el editor y
# Funshi CLI. Estas bibliotecas no dependen de la GUI ni de la pila gráfica.
list(REMOVE_ITEM SOURCES
    ${SRC_DIR}/Configuracion/ProjectManager.cpp
    ${SRC_DIR}/Configuracion/ProjectPaths.cpp
    ${SRC_DIR}/Diagnosticos/RegistroDiagnosticos.cpp
    ${SRC_DIR}/ExcepcionesCPP/RuntimeException.cpp)
# El backend CEF exige C++20 (headers de CEF) y el motor compila en C++17,asi
# que vive en su propia unidad/target (funshi_webui_cef) y no entra en la GLOB.
list(REMOVE_ITEM SOURCES ${SRC_DIR}/WebUI/BackendCef.cpp)
# stb_image (vendoriado en External/stb_image): decodificador de imagenes. Su
# implementacion va en una unidad propia (fuera de src/) para que ningun modulo
# del motor sea dueno de la libreria de terceros.
list(APPEND SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/External/stb_image/stb_image_impl.cpp)

# Kromax graphics API library
find_package(Kromax REQUIRED CONFIG)

# Add Kromax include directories
target_include_directories(funshi_project_core PRIVATE
    ${KROMAX_INCLUDE_DIRS}
)

# Link Kromax library to the core
target_link_libraries(funshi_project_core PRIVATE
    kromax)

# Nuevo backend Kromax
file(GLOB_RECURSE KROMAX_BACKEND_SOURCES
    ${SRC_DIR}/Rendering/Backend/KromaxBackend.cpp)
    
list(APPEND SOURCES ${KROMAX_BACKEND_SOURCES})

# End Kromax section

add_library(funshi_project_core STATIC
    ${SRC_DIR}/Configuracion/ProjectManager.cpp
    ${SRC_DIR}/Configuracion/ProjectPaths.cpp)
target_include_directories(funshi_project_core PUBLIC ${SRC_DIR})

add_library(funshi_diagnostics_core STATIC
    ${SRC_DIR}/Diagnosticos/RegistroDiagnosticos.cpp
    ${SRC_DIR}/ExcepcionesCPP/RuntimeException.cpp)
target_include_directories(funshi_diagnostics_core
    PUBLIC ${SRC_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/External)
target_link_libraries(funshi_diagnostics_core PUBLIC funshi_project_core)
if(WIN32)
    target_link_libraries(funshi_diagnostics_core PUBLIC dbghelp)
else()
    target_link_libraries(funshi_diagnostics_core PUBLIC ${CMAKE_DL_LIBS})
endif()

# miniz (vendoriado en External/miniz): biblioteca ZIP en C, sin dependencias.
# La comparten la libreria del motor (sistema de mods) y los targets de test
# headless que corren con BUILD_ENGINE=OFF. Se expone el directorio External
# para incluirla como <miniz/miniz.h>, igual que nlohmann/json.
add_library(funshi_miniz STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/External/miniz/miniz.c
    ${CMAKE_CURRENT_SOURCE_DIR}/External/miniz/miniz_tdef.c
    ${CMAKE_CURRENT_SOURCE_DIR}/External/miniz/miniz_tinfl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/External/miniz/miniz_zip.c)
target_include_directories(funshi_miniz PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/External)

# El motor se compila como libreria estatica (funshi_engine) y el ejecutable
# solo aporta el punto de entrada (main.cpp). Asi el engine es reutilizable
# (tests, herramientas, otros front-ends) sin recompilar todo. CI puede compilar
# SOLO las pruebas headless con BUILD_ENGINE=OFF.
option(BUILD_ENGINE "Build engine library and executable" ON)
# Soporte de scripts Java via JNI: requiere JDK en tiempo de build para jni.h.
# El JVM en runtime se carga dinamicamente solo si hay una escena con .java.
# Por defecto ON: si el build encuentra el JDK se habilita solo, si no, el
# backend Java se omite sin romper la compilacion de nada.
option(FUNSHI_JAVA "Soporte de scripts Java via JNI (requiere JDK en build)" ON)
# Soporte de interfaces de juego web (HUD en HTML/CSS/JS). El contrato
# (IWebUIBackend) y el backend nulo se compilan siempre; esta opcion habilita los
# backends web reales (p. ej. CEF) cuando el motor se enlaza con sus
# dependencias. Sin ningun backend real el runtime cae al nulo y no dibuja nada.
option(FUNSHI_WEBUI "Soporte de interfaces web (backend CEF)" ON)
# Soporte de backend CEF concreto (consume distribucion CEF descargada en
# configure). Requiere FUNSHI_WEBUI=ON.
option(FUNSHI_CEF "Compilar backend CEF real" OFF)
# Build runtime-only: solo la libreria del motor, sin ejecutable del editor ni
# objetivos de prueba. Lo usa el exportador de juegos para compilar el binario
# standalone (add_subdirectory de la raiz del engine con esta opcion FORZADA).
option(BUILD_RUNTIME "Construir solo la libreria del motor (sin editor ni pruebas)" OFF)

if(BUILD_ENGINE)
    # main.cpp y main_juego.cpp son puntos de entrada, no parte de la libreria.
    list(REMOVE_ITEM SOURCES ${SRC_DIR}/main.cpp)
    list(REMOVE_ITEM SOURCES ${SRC_DIR}/Runtime/main_juego.cpp)

    # El backend Java se compila cuando FUNSHI_JAVA=ON y hay JDK disponible.
    # Debe ir en la libreria principal (funshi_engine) para que los juegos
    # exportados tengan soporte de scripts Java. El target scripts-java-tests
EOF
cat /tmp/cmake_original.txt >> /home/ggmaster/Desktop/Projects/ORG/FunshiEngine/FunshiEngineKromax/FunshiEngineKX/CMakeLists.txt
