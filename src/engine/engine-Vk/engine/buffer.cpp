// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "buffer.h"

#include "device.h"
#include "vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        buffer::buffer(device& device, std::uint32_t size, vk::BufferUsageFlags flags)
            : m_device(device), m_memSize(size) {
            vk::BufferCreateInfo createInfo(vk::BufferCreateFlags(), size, flags, vk::SharingMode::eExclusive);

            m_buffer = m_device.m_logicalDevice.createBuffer(createInfo);
            vk::MemoryRequirements requirements = m_device.m_logicalDevice.getBufferMemoryRequirements(m_buffer);
            vk::MemoryAllocateInfo allocateInfo(
                requirements.size,
                findMemoryType(requirements.memoryTypeBits,
                               vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent));

            m_bufferMemory = device.m_logicalDevice.allocateMemory(allocateInfo);
            m_device.m_logicalDevice.bindBufferMemory(m_buffer, m_bufferMemory, 0);
        }

        buffer::~buffer() {
            m_device.m_logicalDevice.destroyBuffer(m_buffer);
            m_device.m_logicalDevice.freeMemory(m_bufferMemory);
        }

        vk::Buffer buffer::get_handle() const {
            return m_buffer;
        }

        std::uint32_t buffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags propertyFlags) const {
            auto properties = m_device.m_physicalDevice.getMemoryProperties();

            for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) &&
                    (properties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
                    return i;
                }
            }

            throw vulkan_exception("failed to find suitable memory type!");
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst