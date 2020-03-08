// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "image.h"

#include "util.h"

namespace tst {
namespace engine {
    namespace vulkan {

        std::tuple<vk::Image, VkDeviceMemory> create_image(const vk::Device device,
                                                           const vk::Extent2D extent,
                                                           const vk::Format format,
                                                           const vk::ImageTiling tiling,
                                                           const vk::ImageUsageFlags usage,
                                                           const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                                                           const vk::MemoryPropertyFlags memoryFlags) {
            vk::ImageCreateInfo createInfo(vk::ImageCreateFlags(),
                                           vk::ImageType::e2D,
                                           format,
                                           {extent.width, extent.height, 1},
                                           1,
                                           1,
                                           vk::SampleCountFlagBits::e1,
                                           tiling,
                                           usage,
                                           vk::SharingMode::eExclusive,
                                           0,
                                           nullptr,
                                           vk::ImageLayout::eUndefined);
            auto image = device.createImage(createInfo);

            vk::MemoryRequirements requirements = device.getImageMemoryRequirements(image);

            vk::MemoryAllocateInfo allocInfo(
                requirements.size, find_memory_type(memoryProperties, requirements.memoryTypeBits, memoryFlags));

            auto memory = device.allocateMemory(allocInfo);

            device.bindImageMemory(image, memory, 0);

            return {image, memory};
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst