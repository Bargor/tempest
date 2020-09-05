// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

#include "../vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vertex_buffer::vertex_buffer(const buffer::creation_info& info,
                                     const vertex_format& format,
                                     std::vector<vertex>&& vertices)
            : buffer(info,
                     vertices.size() * sizeof(vertex),
                     vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal)
            , m_format(std::move(format))
            , m_vertices(std::move(vertices)) {
            buffer stagingBuffer(info,
                                 m_memSize,
                                 vk::BufferUsageFlagBits::eTransferSrc,
                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

            stagingBuffer.copy_data(m_vertices.data(), m_memSize);
            stagingBuffer.copy_buffer(m_buffer, m_memSize);
        }

        vertex_buffer::~vertex_buffer() {
        }

        vertex_buffer::vertex_buffer(vertex_buffer&& other) noexcept
            : buffer(std::move(other)), m_format(other.m_format), m_vertices(std::move(other.m_vertices)) {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
