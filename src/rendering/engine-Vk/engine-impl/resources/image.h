// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <common/rectangle.h>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {
        class buffer;

        std::tuple<vk::Image, vk::DeviceMemory> create_image(const vk::Device device,
                                                             const vk::Extent2D extent,
                                                             const vk::Format format,
                                                             const vk::ImageTiling tiling,
                                                             const vk::ImageUsageFlags usage,
                                                             const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                                                             const vk::MemoryPropertyFlags memoryFlags);

        vk::ImageView
        create_image_view(vk::Device device, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags);

        void transition_image_layout(vk::Device device,
                                     vk::Queue queue,
                                     vk::CommandPool cmdPool,
                                     vk::Image image,
                                     vk::Format format,
                                     vk::ImageLayout oldLayout,
                                     vk::ImageLayout newLayout);

        void copy_buffer_to_image(vk::Device device,
                                  vk::CommandPool cmdPool,
                                  vk::Queue queue,
                                  const buffer& buffer,
                                  vk::Image image,
                                  core::extent<std::uint32_t> extent);

    } // namespace vulkan
} // namespace engine
} // namespace tst