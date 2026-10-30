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
#ifndef KROMAXBACKEND_H
#define KROMAXBACKEND_H

#include "IRenderBackend.h"

#include <kromax/kromax.h>

#include <memory>
#include <vector>
#include <string>

namespace Rendering {
namespace Backend {

/**
 * @brief Implementación del backend gráfico usando la API Kromax.
 * 
 * Esta clase implementa la interfaz IRenderBackend utilizando Kromax,
 * que es una API gráfica similar a Vulkan.
 */
class KromaxBackend final : public IRenderBackend {
public:
    KromaxBackend();
    ~KromaxBackend() override = default;

    // Copy/move disabled
    KromaxBackend(const KromaxBackend&) = delete;
    KromaxBackend& operator=(const KromaxBackend&) = delete;
    KromaxBackend(KromaxBackend&&) = delete;
    KromaxBackend& operator=(KromaxBackend&&) = delete;

    /** @inheritdoc */
    bool init(void* ventanaNativa) override;
    /** @inheritdoc */
    bool available() const override;

    /** @inheritdoc */
    void beginFrame() override;
    /** @inheritdoc */
    void endFrame() override;

    /** @inheritdoc */
    Handle createMesh(const MeshData& data) override;
    /** @inheritdoc */
    void destroyMesh(Handle mesh) override;
    /** @inheritdoc */
    void drawMesh(Handle mesh, unsigned int indexCount) override;

    /** @inheritdoc */
    Handle createLineBatch(const float* vertices, std::size_t vertexCount) override;
    /** @inheritdoc */
    void destroyLineBatch(Handle batch) override;
    /** @inheritdoc */
    void updateLineBatch(Handle batch, const float* vertices, std::size_t vertexCount) override;
    /** @inheritdoc */
    void drawLineBatch(Handle batch, unsigned int vertexCount) override;

    /** @inheritdoc */
    Handle createTexture2D(const Image2D& image) override;
    /** @inheritdoc */
    void destroyTexture2D(Handle texture) override;
    /** @inheritdoc */
    void bindTexture2D(Handle texture, int unit) override;
    /** @inheritdoc */
    void updateTexture2D(Handle texture, const Image2D& image) override;

    /** @inheritdoc */
    Handle createTextureCube(const ImageCube& image) override;
    /** @inheritdoc */
    void destroyTextureCube(Handle texture) override;
    /** @inheritdoc */
    void bindTextureCube(Handle texture, int unit) override;

    /** @inheritdoc */
    Handle createProgram(Contrato::ShaderKind kind) override;
    /** @inheritdoc */
    void destroyProgram(Handle program) override;
    /** @inheritdoc */
    void useProgram(Handle program) override;
    /** @inheritdoc */
    int uniformLocation(Handle program, const char* name) override;
    /** @inheritdoc */
    void setUniformMat4(int location, const glm::mat4& value) override;
    /** @inheritdoc */
    void setUniformMat3(int location, const glm::mat3& value) override;
    /** @inheritdoc */
    void setUniformVec3(int location, const glm::vec3& value) override;
    /** @inheritdoc */
    void setUniformVec4(int location, const float* value) override;
    /** @inheritdoc */
    void setUniformFloat(int location, float value) override;
    /** @inheritdoc */
    void setUniformInt(int location, int value) override;

    /** @inheritdoc */
    Handle createRenderTarget(int width, int height) override;
    /** @inheritdoc */
    void destroyRenderTarget(Handle target) override;
    /** @inheritdoc */
    void bindRenderTarget(Handle target) override;
    /** @inheritdoc */
    void bindDefaultFramebuffer() override;
    /** @inheritdoc */
    Handle renderTargetColorTexture(Handle target) const override;
    /** @inheritdoc */
    void* imguiTextureId(Handle texture) const override;

    /** @inheritdoc */
    void setViewport(int x, int y, int width, int height) override;
    /** @inheritdoc */
    void clearScreen(const float color[3]) override;
    /** @inheritdoc */
    void applyBaseState() override;
    /** @inheritdoc */
    void setClearColor(const float color[3]) override;
    /** @inheritdoc */
    const char* diagnosticoGPU() const override;
    /** @inheritdoc */
    void setBlendEnabled(bool enabled) override;

    /** @inheritdoc */
    void setDepthTestEnabled(bool enabled) override;
    /** @inheritdoc */
    void setDepthMask(bool enabled) override;
    /** @inheritdoc */
    void setDepthFunc(DepthFunc func) override;

    /** @inheritdoc */
    void drawFullscreenTriangle() override;

private:
    // Inicializar Kromax (instancia y dispositivo)
    bool initKromax(void* ventanaNativa);
    
    // Limpieza
    void cleanupKromax();
    
    // Helper para crear mallas
    Handle createMeshImpl(const MeshData& data);
    
    // Helper para crear texturas
    Handle createTexture2DImpl(const Image2D& image);
    Handle createTextureCubeImpl(const ImageCube& image);
    
    // Helper para crear programas
    Handle createProgramImpl(Contrato::ShaderKind kind);
    
    // Estado actual
    struct ImplState;
    std::unique_ptr<ImplState> pImpl_;
};

} // namespace Backend
} // namespace Rendering

#endif // KROMAXBACKEND_H
