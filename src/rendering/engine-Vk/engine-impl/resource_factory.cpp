// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

namespace tst {
namespace engine {
    namespace vulkan {

        resource_factory::resource_factory(device& device)
            : m_device(device), m_commandPool(m_device.create_command_pool()) {
        }

        resource_factory::~resource_factory() {
        }

        pipeline resource_factory::create_pipeline(const vertex_format& format) {
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
