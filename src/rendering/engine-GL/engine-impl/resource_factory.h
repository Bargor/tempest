// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "api.h"
#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/shader.h"
#include "resources/texture.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/vertex_format.h"

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
                                                 shader_type type,
                                                 base::uniform_bind_point bindPoint,
                                                 std::uint32_t binding,
                                                 std::size_t storageSize);

        private:
            const application::data_loader& m_dataLoader;
            ptr<shader_compiler> m_shaderCompiler;
        };

        template<typename IndexType>
        index_buffer<IndexType> resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
            return index_buffer<std::uint16_t>(std::move(indices));
        }
    } // namespace opengl
} // namespace engine
} // namespace tst