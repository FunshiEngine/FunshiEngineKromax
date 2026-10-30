/*
    FunshiEngineC - Motor de juegos 3D con API grafica intercambiable e ImGui
    Copyright 2026 Gianfranco Ivan Enrique

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    SPDX-License-Identifier: Apache-2.0
*/
#include "KromaxBackend.h"

#include "IRenderBackend.h"
#include <glm/glm.hpp>

#include <kromax/src/api/api_instance.h>
#include <kromax/src/api/api_device.h>
#include <kromax/src/api/api_memory.h>
#include <kromax/src/api/api_buffer.h>
#include <kromax/src/api/api_image.h>
#include <kromax/src/api/api_descriptor.h>
#include <kromax/src/api/api_pipeline.h>
#include <kromax/src/api/api_renderpass.h>
#include <kromax/include/kromax.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace Rendering {
namespace Backend {

// -----------------------------------------------------------------------------
// KromaxBackend::ImplState
// -----------------------------------------------------------------------------
struct KromaxBackend::ImplState {
    // Instancia Kromax
    KxInstance kxInstance_ = nullptr;
    KxPhysicalDevice kxPhysicalDevice_ = nullptr;
    KxDevice kxDevice_ = nullptr;
    
    // Queue families
    uint32_t graphicsQueueFamily_ = VK_QUEUE_FAMILY_IGNORED;
    uint32_t presentQueueFamily_ = VK_QUEUE_FAMILY_IGNORED;
    
    // Queues
    KxQueue graphicsQueue_ = VK_NULL_HANDLE;
    KxQueue presentQueue_ = VK_NULL_HANDLE;
    
    // Estado de render
    bool framebufferResized_ = false;
    
    // Para ImGui
    bool imguiInitialized_ = false;
};

KromaxBackend::KromaxBackend() : pImpl_(std::make_unique<ImplState>()) {}

KromaxBackend::~KromaxBackend() = default;

bool KromaxBackend::init(void* ventanaNativa) {
    // TODO: Aquí integraríamos la creación real de la instancia Kromax
    // usando la ventana nativa (GLFWwindow*).
    // 
    // Por ahora, inicializamos con el backend nulo y marcamos que no hay API
    // Kromax disponible en este entorno.
    (void)ventanaNativa;
    return false;
}

bool KromaxBackend::available() const {
    // Retornar false porque el backend Kromax no está completamente
    // integrado en este momento.
    return false;
}

void KromaxBackend::beginFrame() {
    // Sin API gráfica activa, no hay frame que abrir.
}

void KromaxBackend::endFrame() {
    // Sin API gráfica activa, nothing to send.
}

Handle KromaxBackend::createMesh(const MeshData& data) {
    // Implementación simplificada: retorna handle inválido
    (void)data;
    return kInvalidHandle;
}

void KromaxBackend::destroyMesh(Handle mesh) {
    (void)mesh;
}

void KromaxBackend::drawMesh(Handle mesh, unsigned int indexCount) {
    (void)mesh;
    (void)indexCount;
}

Handle KromaxBackend::createLineBatch(const float* vertices, std::size_t vertexCount) {
    (void)vertices;
    (void)vertexCount;
    return kInvalidHandle;
}

void KromaxBackend::destroyLineBatch(Handle batch) {
    (void)batch;
}

void KromaxBackend::updateLineBatch(Handle batch, const float* vertices, std::size_t vertexCount) {
    (void)batch;
    (void)vertices;
    (void)vertexCount;
}

void KromaxBackend::drawLineBatch(Handle batch, unsigned int vertexCount) {
    (void)batch;
    (void)vertexCount;
}

Handle KromaxBackend::createTexture2D(const Image2D& image) {
    // Implementación simplificada
    (void)image;
    return kInvalidHandle;
}

void KromaxBackend::destroyTexture2D(Handle texture) {
    (void)texture;
}

void KromaxBackend::bindTexture2D(Handle texture, int unit) {
    (void)texture;
    (void)unit;
}

void KromaxBackend::updateTexture2D(Handle texture, const Image2D& image) {
    (void)texture;
    (void)image;
}

Handle KromaxBackend::createTextureCube(const ImageCube& image) {
    (void)image;
    return kInvalidHandle;
}

void KromaxBackend::destroyTextureCube(Handle texture) {
    (void)texture;
}

void KromaxBackend::bindTextureCube(Handle texture, int unit) {
    (void)texture;
    (void)unit;
}

Handle KromaxBackend::createProgram(Contrato::ShaderKind kind) {
    (void)kind;
    return kInvalidHandle;
}

void KromaxBackend::destroyProgram(Handle program) {
    (void)program;
}

void KromaxBackend::useProgram(Handle program) {
    (void)program;
}

int KromaxBackend::uniformLocation(Handle program, const char* name) {
    (void)program;
    (void)name;
    return -1;
}

void KromaxBackend::setUniformMat4(int location, const glm::mat4& value) {
    (void)location;
    (void)value;
}

void KromaxBackend::setUniformMat3(int location, const glm::mat3& value) {
    (void)location;
    (void)value;
}

void KromaxBackend::setUniformVec3(int location, const glm::vec3& value) {
    (void)location;
    (void)value;
}

void KromaxBackend::setUniformVec4(int location, const float* value) {
    (void)location;
    (void)value;
}

void KromaxBackend::setUniformFloat(int location, float value) {
    (void)location;
    (void)value;
}

void KromaxBackend::setUniformInt(int location, int value) {
    (void)location;
    (void)value;
}

Handle KromaxBackend::createRenderTarget(int width, int height) {
    (void)width;
    (void)height;
    return kInvalidHandle;
}

void KromaxBackend::destroyRenderTarget(Handle target) {
    (void)target;
}

void KromaxBackend::bindRenderTarget(Handle target) {
    (void)target;
}

void KromaxBackend::bindDefaultFramebuffer() {
}

Handle KromaxBackend::renderTargetColorTexture(Handle target) const {
    (void)target;
    return kInvalidHandle;
}

void* KromaxBackend::imguiTextureId(Handle texture) const {
    (void)texture;
    return nullptr;
}

void KromaxBackend::setViewport(int x, int y, int width, int height) {
    (void)x;
    (void)y;
    (void)width;
    (void)height;
}

void KromaxBackend::clearScreen(const float color[3]) {
    (void)color;
}

void KromaxBackend::applyBaseState() {
}

void KromaxBackend::setClearColor(const float color[3]) {
    (void)color;
}

const char* KromaxBackend::diagnosticoGPU() const {
    return "[Render] Backend Kromax: API no implementada completamente en esta versión.";
}

void KromaxBackend::setBlendEnabled(bool enabled) {
    (void)enabled;
}

void KromaxBackend::setDepthTestEnabled(bool enabled) {
    (void)enabled;
}

void KromaxBackend::setDepthMask(bool enabled) {
    (void)enabled;
}

void KromaxBackend::setDepthFunc(DepthFunc func) {
    (void)func;
}

void KromaxBackend::drawFullscreenTriangle() {
}

// -----------------------------------------------------------------------------
// Constructor / Destructor de singleton/active backend
// -----------------------------------------------------------------------------

// Instancia el backend Kromax y lo instala como backend activo
std::unique_ptr<IRenderBackend> crearKromaxBackend() {
    return std::make_unique<KromaxBackend>();
}

// Registrar la fábrica para que sea usada por instalarBackend
// (esto se conectaría con la función instalarBackend definida en IRenderBackend.h)

} // namespace Backend
} // namespace Rendering
