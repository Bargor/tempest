// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "buffer.h"

#include "device.h"
#include "vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        buffer::buffer(device& device,
                       vk::CommandPool& cmdPool,
                       std::uint32_t size,
                       vk::BufferUsageFlags usageFlags,
                       vk::MemoryPropertyFlags memoryFlags)
            : m_device(device), m_cmdPool(cmdPool), m_memSize(size) {
            vk::BufferCreateInfo createInfo(vk::BufferCreateFlags(), size, usageFlags, vk::SharingMode::eExclusive);

            m_buffer = m_device.m_logicalDevice.createBuffer(createInfo);
            vk::MemoryRequirements requirements = m_device.m_logicalDevice.getBufferMemoryRequirements(m_buffer);
            vk::MemoryAllocateInfo allocateInfo(requirements.size,
                                                findMemoryType(requirements.memoryTypeBits, memoryFlags));

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

        void buffer::copy_data(void* data, std::uint32_t size) {
            auto dataPtr = m_device.m_logicalDevice.mapMemory(m_bufferMemory, 0, m_memSize);
            std::memcpy(dataPtr, data, size);
            m_device.m_logicalDevice.unmapMemory(m_bufferMemory);
        }

        void buffer::copy_buffer(vk::Buffer& dstBuffer, std::uint32_t size) const {
            vk::CommandBufferAllocateInfo allocInfo(m_cmdPool, vk::CommandBufferLevel::ePrimary, 1);

            auto cmdBuffer = m_device.m_logicalDevice.allocateCommandBuffers(allocInfo);

            vk::CommandBufferBeginInfo cmdBufferInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

            cmdBuffer[0].begin(cmdBufferInfo);
            { 
				vk::BufferCopy copyInfo(0, 0, size); 
				cmdBuffer[0].copyBuffer(m_buffer, dstBuffer, copyInfo);
			}
            cmdBuffer[0].end();

			vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &cmdBuffer[0]);
            m_device.m_graphicsQueueHandle.submit({submitInfo}, vk::Fence());
            m_device.m_graphicsQueueHandle.waitIdle();
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