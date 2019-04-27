// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

#include "vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vertex_buffer::vertex_buffer(const vk::Device& device,
                                     const vk::PhysicalDevice& physicalDevice,
                                     const vertex_format&,
                                     std::vector<vertex>&& vertices)
            : m_device(device), m_physicalDevice(physicalDevice) {
            vk::BufferCreateInfo createInfo(vk::BufferCreateFlags(),
                                            sizeof(vertex) * vertices.size(),
                                            vk::BufferUsageFlagBits::eVertexBuffer,
                                            vk::SharingMode::eExclusive);
            m_buffer = m_device.createBuffer(createInfo);

            vk::MemoryRequirements requirements = device.getBufferMemoryRequirements(m_buffer);
            vk::MemoryAllocateInfo allocateInfo(
                requirements.size,
                findMemoryType(requirements.memoryTypeBits,
                               vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent));

			m_bufferMemory = device.allocateMemory(allocateInfo);
            device.bindBufferMemory(m_buffer, m_bufferMemory, 0);

			auto dataPtr = device.mapMemory(m_bufferMemory, 0, createInfo.size);
            std::memcpy(dataPtr, vertices.data(), createInfo.size);
            device.unmapMemory(m_bufferMemory);
        }

        vertex_buffer::~vertex_buffer() {
            m_device.destroyBuffer(m_buffer);
            m_device.freeMemory(m_bufferMemory);
        }

        std::uint32_t vertex_buffer::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags propertyFlags) const {
            auto properties = m_physicalDevice.getMemoryProperties();

            for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) &&
                    (properties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
                    return i;
                }
            }

            throw vulkan_exception("failed to find suitable memory type!");
        }

		vk::Buffer vertex_buffer::get_handle() const {
            return m_buffer;
		}
    } // namespace vulkan
} // namespace engine
} // namespace tst