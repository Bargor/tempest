// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "api.h"
#include "resources/index_buffer.h"
#include "resources/material.h"
#include "resources/pipeline.h"
#include "resources/shader.h"
#include "resources/texture.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/vertex_format.h"

#include <engine-base/resource_bind_point.h>

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {

    namespace opengl {
        class shader_compiler;

        class resource_factory {
        public:
            resource_factory(const application::data_loader& dataLoader);
            ~resource_factory();

            resource_factory(const resource_factory&) = delete;
            resource_factory(resource_factory&& factory) noexcept;

        public:
            pipeline::pipeline_hash create_pipeline(const std::string& techniqueName,
                                                    const std::string_view pipelineName,
                                                    const std::string& shadersName,
                                                    const vertex_buffer& vertexBuffer);
            void create_technique(std::string&& name);
            texture create_texture(const std::string& textureName);
            uniform_buffer create_uniform_buffer(const std::string& shaderName,
                                                 base::resource_bind_point bindPoint,
                                                 std::uint32_t binding,
                                                 std::size_t storageSize);

            material create_material(std::string&& materialName,
                                     const std::string& shaderName,
                                     const std::vector<std::string>& textureNames,
                                     std::uint32_t staticStorageSize,
                                     std::uint32_t dynamicStorageSize);

            buffer::creation_info create_buffer_creation_info() const noexcept;
            uniform_buffer::creation_info create_uniform_creation_info(const std::string& shaderName,
                                                                       base::resource_bind_point bindPoint) const noexcept;

            texture::creation_info create_texture_creation_info(const std::string& textureName) const;

            material::creation_info
            resource_factory::create_material_creation_info(const std::string& shaderName,
                                                            const std::vector<std::string>& textureNames,
                                                            std::uint32_t staticStorageSize,
                                                            std::uint32_t dynamicStorageSize) const;
        private:
            const application::data_loader& m_dataLoader;
            ptr<shader_compiler> m_shaderCompiler;
            std::vector<pipeline> m_pipelines;
        };
    } // namespace opengl
} // namespace engine
} // namespace tst