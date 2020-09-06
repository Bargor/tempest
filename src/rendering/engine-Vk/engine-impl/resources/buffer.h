// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class buffer {
        public:
            struct creation_info {
                vk::Device logicalDevice;
                vk::Queue queueHandle;
                vk::CommandPool cmdPool;
                vk::PhysicalDeviceMemoryProperties memoryProperties;
            };

        public:
            buffer(const creation_info& info,
                   std::size_t size,
                   vk::BufferUsageFlags flags,
                   vk::MemoryPropertyFlags memoryFlags);
            ~buffer();

            buffer(buffer&& other) noexcept;
            buffer(const buffer&) = delete;

            vk::Buffer get_handle() const;
            void copy_data(void* data, std::uint64_t size);
            void copy_buffer(vk::Buffer& dstBuffer, std::uint64_t size) const;

        protected:
            vk::Device m_logicalDevice;
            vk::Queue m_queueHandle;
            vk::CommandPool m_cmdPool;
            std::uint64_t m_memSize;
            vk::Buffer m_buffer;
            vk::DeviceMemory m_bufferMemory;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
