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
            buffer(const vk::Device& logicalDevice,
                   const vk::PhysicalDevice& physicalDevice,
				   const vk::Queue& queueHandle,
				   const vk::CommandPool& cmdPool,
                   std::uint32_t size,
                   vk::BufferUsageFlags flags,
                   vk::MemoryPropertyFlags memoryFlags);
            ~buffer();

            buffer(buffer&& other) noexcept;
            buffer(const buffer&) = delete;

            vk::Buffer get_handle() const;
            void copy_data(void* data, std::uint32_t size);
            void copy_buffer(vk::Buffer& dstBuffer, std::uint32_t size) const;

        protected:
            const vk::Device& m_logicalDevice;
            const vk::PhysicalDevice& m_physicalDevice;
            const vk::Queue& m_queueHandle;
            const vk::CommandPool& m_cmdPool;
            std::uint32_t m_memSize;
            vk::Buffer m_buffer;
            vk::DeviceMemory m_bufferMemory;

        private:
            std::uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst