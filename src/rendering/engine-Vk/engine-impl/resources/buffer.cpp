// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "buffer.h"
#include "util.h"

namespace tst {
namespace engine {
    namespace vulkan {

        class physical_device;

        buffer::buffer(vk::Device logicalDevice,
                       vk::Queue queueHandle,
                       vk::CommandPool cmdPool,
                       std::uint64_t size,
                       vk::BufferUsageFlags usageFlags,
                       const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                       vk::MemoryPropertyFlags memoryFlags)
            : m_logicalDevice(logicalDevice), m_queueHandle(queueHandle), m_cmdPool(cmdPool), m_memSize(size) {
            const vk::BufferCreateInfo createInfo(vk::BufferCreateFlags(), size, usageFlags, vk::SharingMode::eExclusive);

            m_buffer = m_logicalDevice.createBuffer(createInfo);
            const auto requirements = m_logicalDevice.getBufferMemoryRequirements(m_buffer);
            const vk::MemoryAllocateInfo allocateInfo(
                requirements.size, find_memory_type(memoryProperties, requirements.memoryTypeBits, memoryFlags));

            m_bufferMemory = m_logicalDevice.allocateMemory(allocateInfo);
            m_logicalDevice.bindBufferMemory(m_buffer, m_bufferMemory, 0);
        }

        buffer::~buffer() {
            if (m_buffer) {
                m_logicalDevice.destroyBuffer(m_buffer);
                m_logicalDevice.freeMemory(m_bufferMemory);
            }
        }

        buffer::buffer(buffer&& other) noexcept
            : m_logicalDevice(other.m_logicalDevice)
            , m_queueHandle(other.m_queueHandle)
            , m_cmdPool(other.m_cmdPool)
            , m_memSize(other.m_memSize)
            , m_buffer(other.m_buffer)
            , m_bufferMemory(other.m_bufferMemory) {
            other.m_buffer = nullptr;
            other.m_bufferMemory = nullptr;
        }

        vk::Buffer buffer::get_handle() const {
            return m_buffer;
        }

        void buffer::copy_data(void* data, std::uint64_t size) {
            auto dataPtr = m_logicalDevice.mapMemory(m_bufferMemory, 0, m_memSize);
            std::memcpy(dataPtr, data, size);
            m_logicalDevice.unmapMemory(m_bufferMemory);
        }

        void buffer::copy_buffer(vk::Buffer& dstBuffer, std::uint64_t size) const {
            vk::CommandBufferAllocateInfo allocInfo(m_cmdPool, vk::CommandBufferLevel::ePrimary, 1);

            auto cmdBuffer = m_logicalDevice.allocateCommandBuffers(allocInfo);

            vk::CommandBufferBeginInfo cmdBufferInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

            cmdBuffer[0].begin(cmdBufferInfo);
            {
                vk::BufferCopy copyInfo(0, 0, size);
                cmdBuffer[0].copyBuffer(m_buffer, dstBuffer, copyInfo);
            }
            cmdBuffer[0].end();

            vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &cmdBuffer[0]);
            m_queueHandle.submit({submitInfo}, vk::Fence());
            m_queueHandle.waitIdle();
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
