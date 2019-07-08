// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "../vulkan_exception.h"
#include "buffer.h"
#include "vertex_format.h"

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        template<typename IndexType>
        class index_buffer : public buffer {
        public:
        public:
            index_buffer(const vk::Device& logicalDevice,
                         const vk::PhysicalDevice& physicalDevice,
                         const vk::Queue& queueHandle,
                         const vk::CommandPool& cmdPool,
                         vk::IndexType format,
                         std::vector<IndexType>&& indices);
            ~index_buffer();

            std::uint32_t get_index_count() const;

        private:
            vk::IndexType m_format;
            std::vector<IndexType> m_indices;
        };

        template<typename IndexType>
        index_buffer<IndexType>::index_buffer(const vk::Device& logicalDevice,
                                              const vk::PhysicalDevice& physicalDevice,
                                              const vk::Queue& queueHandle,
                                              const vk::CommandPool& cmdPool,
                                              vk::IndexType format,
                                              std::vector<IndexType>&& indices)
            : buffer(logicalDevice,
                     physicalDevice,
                     queueHandle,
                     cmdPool,
                     static_cast<std::uint32_t>(indices.size()) * sizeof(vertex),
                     vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                     vk::MemoryPropertyFlagBits::eDeviceLocal)
            , m_format(format)
            , m_indices(indices) {
            buffer stagingBuffer(logicalDevice,
                                 physicalDevice,
                                 queueHandle,
                                 cmdPool,
                                 m_memSize,
                                 vk::BufferUsageFlagBits::eTransferSrc,
                                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

            stagingBuffer.copy_data(indices.data(), m_memSize);
            stagingBuffer.copy_buffer(m_buffer, m_memSize);
        }

        template<typename IndexType>
        index_buffer<IndexType>::~index_buffer() {
        }

        template<typename IndexType>
        std::uint32_t index_buffer<IndexType>::get_index_count() const {
            return static_cast<std::uint32_t>(m_indices.size());
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst