// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "resource_cache.h"

namespace tst {
namespace engine {
    namespace vulkan {

        resource_factory::resource_factory(device& device, application::data_loader& dataLoader)
            : m_device(device)
            , m_dataLoader(dataLoader)
            , m_resourceCache(m_device.get_resource_cache())
            , m_commandPool(m_device.create_command_pool()) {
        }

        resource_factory::~resource_factory() {
        }

        pipeline resource_factory::create_pipeline(base::pipeline&& basePipeline,
                                                   const std::string& techniqueName,
                                                   const std::string& shadersName,
                                                   const vertex_format& format) {
            auto shaders = m_resourceCache.find_shaders(shadersName);
            auto technique = m_resourceCache.find_technique(techniqueName);

            if (shaders && technique) {
                pipeline pipeline(std::move(basePipeline), format, *shaders, *technique);

                m_resourceCache.add_pipeline(pipeline);

                return pipeline;
            }
        }

        vertex_buffer resource_factory::create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) {
            return m_device.create_vertex_buffer(format, std::move(vertices), m_commandPool);
        }

        uniform_buffer resource_factory::create_uniform_buffer() {
            return m_device.create_uniform_buffer(m_commandPool);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
