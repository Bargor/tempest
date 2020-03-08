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

        vk::ImageView
        create_image_view(vk::Device device, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);
    } // namespace vulkan
} // namespace engine
} // namespace tst
