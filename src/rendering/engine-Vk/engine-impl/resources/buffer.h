// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class buffer {
            friend class engine;

        public:
            buffer(vk::Device logicalDevice,
                   vk::Queue queueHandle,
                   vk::CommandPool cmdPool,
                   std::uint64_t size,
                   vk::BufferUsageFlags flags,
                   const vk::PhysicalDeviceMemoryProperties& memoryProperties,
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

        private:
            std::uint32_t findMemoryType(const vk::PhysicalDeviceMemoryProperties& properties,
                                         uint32_t typeFilter,
                                         vk::MemoryPropertyFlags propertyFlags) const;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
