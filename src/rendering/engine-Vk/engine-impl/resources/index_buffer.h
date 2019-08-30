// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "../vulkan_exception.h"
#include "buffer.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        template<typename IndexType>
        class index_buffer : public buffer {
        public:
            using super = buffer;

            index_buffer(const vk::Device& logicalDevice,
                         const vk::Queue& queueHandle,
                         const vk::CommandPool& cmdPool,
                         const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                         vk::IndexType format,
                         std::vector<IndexType>&& indices);
            ~index_buffer();

            index_buffer(index_buffer&& other) noexcept;

            std::uint32_t get_index_count() const noexcept;

        private:
            vk::IndexType m_format;
            std::vector<IndexType> m_indices;
        };

        template<typename IndexType>
        index_buffer<IndexType>::index_buffer(const vk::Device& logicalDevice,
                                              const vk::Queue& queueHandle,
                                              const vk::CommandPool& cmdPool,
                                              const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                                              vk::IndexType format,
                                              std::vector<IndexType>&& indices)
            : buffer(logicalDevice,
                     queueHandle,
                     cmdPool,
                     indices.size() * sizeof(IndexType),
                     vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                     memoryProperties,
                     vk::MemoryPropertyFlagBits::eDeviceLocal)
            , m_format(format)
            , m_indices(std::move(indices)) {
            buffer stagingBuffer(logicalDevice,
                                 queueHandle,
                                 cmdPool,
                                 m_memSize,
                                 vk::BufferUsageFlagBits::eTransferSrc,
                                 memoryProperties,
                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

            stagingBuffer.copy_data(m_indices.data(), m_memSize);
            stagingBuffer.copy_buffer(m_buffer, m_memSize);
        }

        template<typename IndexType>
        index_buffer<IndexType>::index_buffer(index_buffer&& other) noexcept
            : buffer(std::move(other)), m_format(other.m_format), m_indices(std::move(other.m_indices)) {
        }

        template<typename IndexType>
        index_buffer<IndexType>::~index_buffer() {
        }

        template<typename IndexType>
        std::uint32_t index_buffer<IndexType>::get_index_count() const noexcept {
            return static_cast<std::uint32_t>(m_indices.size());
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
