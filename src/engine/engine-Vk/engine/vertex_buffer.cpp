// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

#include "device.h"
#include "vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vertex_buffer::vertex_buffer(device& device, const vertex_format&, std::vector<vertex>&& vertices)
            : buffer(device, static_cast<std::uint32_t>(vertices.size()) * sizeof(vertex),
                     vk::BufferUsageFlagBits::eVertexBuffer) {
            auto dataPtr = m_device.m_logicalDevice.mapMemory(m_bufferMemory, 0, m_memSize);
            std::memcpy(dataPtr, vertices.data(), m_memSize);
            m_device.m_logicalDevice.unmapMemory(m_bufferMemory);
        }

        vertex_buffer::~vertex_buffer() {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst