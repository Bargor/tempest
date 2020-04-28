// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {
        std::uint32_t find_memory_type(const vk::PhysicalDeviceMemoryProperties& properties,
                                       uint32_t typeFilter,
                                       vk::MemoryPropertyFlags propertyFlags);

        vk::CommandBuffer create_one_time_buffer(vk::Device device, vk::CommandPool cmdPool);
        void submit_one_time_buffer(vk::Device device, vk::CommandPool cmdPool, vk::Queue queue, vk::CommandBuffer cmdBuffer);

    } // namespace vulkan
} // namespace engine
} // namespace tst
