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

        vk::ImageView
        create_image_view(vk::Device device, vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags) {
            const vk::ImageViewCreateInfo createInfo(vk::ImageViewCreateFlags(),
                                                     image,
                                                     vk::ImageViewType::e2D,
                                                     format,
                                                     vk::ComponentMapping(),
                                                     vk::ImageSubresourceRange(aspectFlags, 0, 1, 0, 1));

            try {
                return device.createImageView(createInfo);
            } catch (std::runtime_error&) {
                throw vulkan_exception("Failed to create image views!");
            }
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst