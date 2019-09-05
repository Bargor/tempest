// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device.h"
#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/vertex_format.h"

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {

    namespace vulkan {

        class device;
        class resource_cache;
        class shader_compiler;

        class resource_factory {
        public:
            resource_factory(device& device, application::data_loader& dataLoader);
            ~resource_factory();

        public: // public resource factory interface
            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<std::uint16_t>&& indices);
            pipeline create_pipeline(base::pipeline&& basePipeline,
                                     const std::string& techniqueName,
                                     const std::string& shadersName,
                                     const vertex_format& format);
            vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
            uniform_buffer create_uniform_buffer();

        public: // vulkan internal
            const shader_set* load_shaders(const std::string& shadersName);

        private:
            device& m_device;
            application::data_loader& m_dataLoader;
            resource_cache& m_resourceCache;
            ptr<shader_compiler> m_shaderCompiler;
            vk::CommandPool m_commandPool;
        };

        template<typename IndexType>
        index_buffer<IndexType> resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
            return m_device.create_index_buffer(std::move(indices), m_commandPool);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
