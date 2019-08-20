// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

#include "../vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        uniform_buffer::uniform_buffer(const vk::Device& logicalDevice,
                                       const vk::Queue queueHandle,
                                       const vk::CommandPool& cmdPool,
                                       const vk::PhysicalDeviceMemoryProperties& memoryProperties)
            : buffer(logicalDevice,
                     queueHandle,
                     cmdPool,
                     sizeof(uniform_buffer_object),
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     memoryProperties,
                     vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible) {
        }

        uniform_buffer::~uniform_buffer() {
        }

        uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept : buffer(std::move(other)) {
        }

        void uniform_buffer::update_buffer(const uniform_buffer_object& ubo) {
            m_data = ubo;
            auto dataPtr = m_logicalDevice.mapMemory(m_bufferMemory, 0, sizeof(uniform_buffer_object));
            std::memcpy(dataPtr, &ubo, sizeof(uniform_buffer_object));
            m_logicalDevice.unmapMemory(m_bufferMemory);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
