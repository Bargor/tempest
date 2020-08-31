// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "../vulkan_exception.h"
#include "buffer.h"

#include <variant>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class index_buffer : public buffer {
            using super = buffer;

        public:
            template<typename IndexType>
            index_buffer(vk::Device logicalDevice,
                         vk::Queue queueHandle,
                         vk::CommandPool cmdPool,
                         const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                         vk::IndexType format,
                         std::vector<IndexType>&& indices);
            ~index_buffer() = default;

            index_buffer(index_buffer&& other) noexcept;

            std::uint32_t get_index_count() const noexcept;
            vk::IndexType get_index_type() const noexcept;

        private:
            vk::IndexType m_format;
            std::variant<std::vector<std::uint16_t>, std::vector<std::uint32_t>> m_indices;
        };

        template<typename IndexType>
        index_buffer::index_buffer(vk::Device logicalDevice,
                                   vk::Queue queueHandle,
                                   vk::CommandPool cmdPool,
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

            stagingBuffer.copy_data(std::visit([](auto&& arg) { return static_cast<void*>(arg.data()); }, m_indices),
                                    m_memSize);
            stagingBuffer.copy_buffer(m_buffer, m_memSize);
        }

        TST_INLINE std::uint32_t index_buffer::get_index_count() const noexcept {
            return std::visit([](auto&& arg) { return static_cast<std::uint32_t>(arg.size()); }, m_indices);
        }

        TST_INLINE vk::IndexType index_buffer::get_index_type() const noexcept {
            return m_format;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
