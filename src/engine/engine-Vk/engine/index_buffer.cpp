// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

#include "device.h"
#include "vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        index_buffer::index_buffer(device& device,
                                   vk::CommandPool& cmdPool,
                                   vk::IndexType format,
                                   std::vector<std::uint16_t>&& indices)
            : buffer(device,
                     cmdPool,
                     static_cast<std::uint32_t>(indices.size()) * sizeof(vertex),
                     vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal)
            , m_format(format)
            , m_indices(indices) {
            buffer stagingBuffer(device,
                                 cmdPool,
                                 m_memSize,
                                 vk::BufferUsageFlagBits::eTransferSrc,
                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

            stagingBuffer.copy_data(indices.data(), m_memSize);
            stagingBuffer.copy_buffer(m_buffer, m_memSize);
        }

        index_buffer::~index_buffer() {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst