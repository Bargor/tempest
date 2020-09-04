// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "index_buffer.h"

#include "../resource_factory.h"

namespace tst {
namespace engine {
    namespace vulkan {

        template<>
        index_buffer::index_buffer(const buffer_construction_info& info, std::vector<std::uint16_t>&& indices)
            : buffer(info,
                     indices.size() * sizeof(std::uint16_t),
                     vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal)
            , m_format(vk::IndexType::eUint16)
            , m_indices(std::move(indices)) {
            copy_to_gpu(info);
        }

        template<>
        index_buffer::index_buffer(const buffer_construction_info& info, std::vector<std::uint32_t>&& indices)
            : buffer(info,
                     indices.size() * sizeof(std::uint32_t),
                     vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal)
            , m_format(vk::IndexType::eUint32)
            , m_indices(std::move(indices)) {
            copy_to_gpu(info);
        }

        index_buffer::index_buffer(index_buffer&& other) noexcept
            : buffer(std::move(other)), m_format(other.m_format), m_indices(std::move(other.m_indices)) {
        }

        void index_buffer::copy_to_gpu(const buffer_construction_info& info) {
            buffer stagingBuffer(info,
                                 m_memSize,
                                 vk::BufferUsageFlagBits::eTransferSrc,
                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

            stagingBuffer.copy_data(std::visit([](auto&& arg) { return static_cast<void*>(arg.data()); }, m_indices),
                                    m_memSize);
            stagingBuffer.copy_buffer(m_buffer, m_memSize);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst