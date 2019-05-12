// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

		class device;

        class buffer {
            friend class engine;

        public:
            buffer(device& device, std::uint32_t size, vk::BufferUsageFlags flags);
            ~buffer();

            vk::Buffer get_handle() const;

        protected:
            device& m_device;
            std::uint32_t m_memSize;
            vk::Buffer m_buffer;
            vk::DeviceMemory m_bufferMemory;

		private:
            std::uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) const;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst