// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

namespace tst {
namespace engine {
    namespace vulkan {
        vertex_buffer::vertex_buffer(const vk::Device& device, const vertex_format&, std::vector<vertex>&& vertices)
            : m_device(device) {
            vk::BufferCreateInfo createInfo(vk::BufferCreateFlags(),
                                            sizeof(vertex) * vertices.size(),
                                            vk::BufferUsageFlagBits::eVertexBuffer,
                                            vk::SharingMode::eExclusive);
            m_buffer = m_device.createBuffer(createInfo);
        }

        vertex_buffer::~vertex_buffer() {
            m_device.destroyBuffer(m_buffer);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst