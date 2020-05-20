// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "api.h"
#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/texture.h"
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
        class physical_device;
        class resource_cache;
        class shader_compiler;
        class swap_chain;

        class resource_factory {
        public:
            resource_factory(const device& device, const application::data_loader& dataLoader);
            ~resource_factory();

            resource_factory(const resource_factory&) = delete;
            resource_factory(resource_factory&& factory) noexcept;

        public: // public resource factory interface
            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<std::uint16_t>&& indices);
            const pipeline& create_pipeline(const std::string& techniqueName,
                                            const std::string& pipelineName,
                                            const std::string& shadersName,
                                            const vertex_format& format);
            void create_technique(const std::string& name);
            vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
            uniform_buffer create_uniform_buffer(const std::string& shaderName,
                                                 shader_type type,
                                                 std::uint32_t binding,
                                                 std::size_t storageSize);
            texture create_texture(const std::string& textureName);

        public: // vulkan internal
            const shader_set* load_shaders(const std::string& shadersName);

        private:
            vk::DescriptorPool create_descriptor_pool(std::uint32_t size);

        private:
            const device& m_device;
            const application::data_loader& m_dataLoader;

            ptr<shader_compiler> m_shaderCompiler;
            vk::CommandPool m_transferCommandPool;
            vk::Queue m_transferQueue;
            std::vector<vk::DescriptorPool> m_descriptorPools;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
