// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "util.h"

#include "../vulkan_exception.h"

namespace tst {
namespace engine {
    namespace vulkan {
        std::uint32_t find_memory_type(const vk::PhysicalDeviceMemoryProperties& properties,
                                       uint32_t typeFilter,
                                       vk::MemoryPropertyFlags propertyFlags) {
            for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) &&
                    (properties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags) {
                    return i;
                }
            }

            throw vulkan_exception("failed to find suitable memory type!");
        }

        vk::CommandBuffer create_one_time_buffer(vk::Device device, vk::CommandPool cmdPool) {
            vk::CommandBufferAllocateInfo allocInfo(cmdPool, vk::CommandBufferLevel::ePrimary, 1);

            auto cmdBuffer = device.allocateCommandBuffers(allocInfo);

            vk::CommandBufferBeginInfo cmdBufferInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

            cmdBuffer[0].begin(cmdBufferInfo);

            return cmdBuffer[0];
        }

        void submit_one_time_buffer(vk::Device device, vk::CommandPool cmdPool, vk::Queue queue, vk::CommandBuffer cmdBuffer)
        {
            cmdBuffer.end();

            vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &cmdBuffer);
            queue.submit({submitInfo}, vk::Fence());
            queue.waitIdle();

            device.freeCommandBuffers(cmdPool, cmdBuffer);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst