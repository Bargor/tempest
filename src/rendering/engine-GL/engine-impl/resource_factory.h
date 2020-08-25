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
            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<std::uint16_t>&& indices);
            const pipeline& create_pipeline(const std::string& techniqueName,
                                            const std::string& pipelineName,
                                            const std::string& shadersName,
                                            const vertex_format& format);
            void create_technique(const std::string& name);
            texture create_texture(const std::string& textureName);
            vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
            uniform_buffer create_uniform_buffer(const std::string& shaderName,
                                                 base::resource_bind_point bindPoint,
                                                 std::uint32_t binding,
                                                 std::size_t storageSize);

            material create_material(const std::string materialName,
                                     const std::string& shaderName,
                                     const std::vector<std::string>& textureNames,
                                     std::uint32_t staticStorageSize,
                                     std::uint32_t dynamicStorageSize);

        private:
            const application::data_loader& m_dataLoader;
            ptr<shader_compiler> m_shaderCompiler;
            std::vector<pipeline> m_pipelines;
        };

        template<typename IndexType>
        index_buffer<IndexType> resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
            return index_buffer<std::uint16_t>(std::move(indices));
        }
    } // namespace opengl
} // namespace engine
} // namespace tst