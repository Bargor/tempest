// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "engine/device.h"

namespace tst {
namespace engine {
    namespace resources {

        resource_factory::resource_factory(api::device& device)
            : m_nativeDevice(device), m_commandPool(m_device.create_command_pool()) {
			
        }

        resource_factory::~resource_factory() {
        }

        index_buffer resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
            return m_nativeDevice.create_index_buffer(std::move(indices), m_commandPool);
        }

		vertex_buffer resource_factory::create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) {
            return m_nativeDevice.create_vertex_buffer(format, std::move(vertices), m_commandPool);
        }

		uniform_buffer resource_factory::create_uniform_buffer() {
            return m_nativeDevice.create_uniform_buffer(m_commandPool);
		}
    } // namespace resources
} // namespace engine
} // namespace tst