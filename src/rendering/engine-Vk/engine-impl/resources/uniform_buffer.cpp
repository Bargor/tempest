// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

#include "../vulkan_exception.h"
#include "settings.h"

namespace tst {
namespace engine {
    namespace vulkan {

        uniform_buffer::uniform_buffer(vk::Device logicalDevice,
                                       vk::Queue queueHandle,
                                       vk::CommandPool cmdPool,
                                       const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                                       const std::uint32_t& resourceIndex)
            : buffer(logicalDevice,
                     queueHandle,
                     cmdPool,
                     sizeof(uniform_buffer_object) * settings::m_inFlightFrames,
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     memoryProperties,
                     vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible)
            , m_resourceIndex(resourceIndex) {
        }

        uniform_buffer::~uniform_buffer() {
        }

        uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept
            : buffer(std::move(other)), m_resourceIndex(other.m_resourceIndex) {
        }

        void uniform_buffer::update_buffer(const uniform_buffer_object& ubo) {
            m_data = ubo;
            auto dataPtr = m_logicalDevice.mapMemory(
                m_bufferMemory, m_resourceIndex * sizeof(uniform_buffer_object), sizeof(uniform_buffer_object));
            std::memcpy(dataPtr, &ubo, sizeof(uniform_buffer_object));
            m_logicalDevice.unmapMemory(m_bufferMemory);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
