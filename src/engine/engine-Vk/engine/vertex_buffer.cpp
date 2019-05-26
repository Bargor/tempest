// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

#include "device.h"
#include "vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vertex_buffer::vertex_buffer(device& device,
                                     vk::CommandPool& cmdPool,
                                     const vertex_format&,
                                     std::vector<vertex>&& vertices)
            : buffer(device,
                     cmdPool, static_cast<std::uint32_t>(vertices.size()) * sizeof(vertex),
                     vk::BufferUsageFlagBits::eVertexBuffer |
                         vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal) {

			buffer stagingBuffer(device,
                                 cmdPool,
                                 m_memSize,
                                 vk::BufferUsageFlagBits::eTransferSrc,
                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

			stagingBuffer.copy_data(vertices.data(), m_memSize);  
			stagingBuffer.copy_buffer(m_buffer, m_memSize);
        }

        vertex_buffer::~vertex_buffer() {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst