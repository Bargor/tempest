// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {
        std::tuple<vk::Image, VkDeviceMemory> create_image(const vk::Device device,
                                                           const vk::Extent2D extent,
                                                           const vk::Format format,
                                                           const vk::ImageTiling tiling,
                                                           const vk::ImageUsageFlags usage,
                                                           const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                                                           const vk::MemoryPropertyFlags memoryFlags);
    }
} // namespace engine
} // namespace tst