// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

#include "device.h"
#include "vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        uniform_buffer::uniform_buffer(const device& device, const vk::CommandPool& cmdPool)
            : buffer(device,
                     cmdPool,
                     sizeof(uniform_buffer_object),
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible) {
        }

        uniform_buffer::~uniform_buffer() {
        }

		uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept : buffer(std::move(other)) {
        
		}

        void uniform_buffer::update_buffer(const uniform_buffer_object& ubo) {
            auto dataPtr = m_device.m_logicalDevice.mapMemory(m_bufferMemory, 0, sizeof(uniform_buffer_object));
            std::memcpy(dataPtr, &ubo, sizeof(uniform_buffer_object));
            m_device.m_logicalDevice.unmapMemory(m_bufferMemory);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst