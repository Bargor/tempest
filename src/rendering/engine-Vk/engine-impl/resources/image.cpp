// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "image.h"

#include "../vulkan_exception.h"
#include "buffer.h"
#include "util.h"

namespace tst {
namespace engine {
    namespace vulkan {

        std::tuple<vk::Image, vk::DeviceMemory> create_image(const vk::Device device,
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
                                           {},
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

        void transition_image_layout(vk::Device device,
                                     vk::Queue queue,
                                     vk::CommandPool cmdPool,
                                     vk::Image image,
                                     vk::Format,
                                     vk::ImageLayout oldLayout,
                                     vk::ImageLayout newLayout) {
            const auto cmdBuffer = create_one_time_buffer(device, cmdPool);

            vk::AccessFlags srcAccessMask;
            vk::AccessFlags dstAccessMask;
            vk::PipelineStageFlags srcStage;
            vk::PipelineStageFlags dstStage;

            if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
                dstAccessMask |= vk::AccessFlagBits::eTransferWrite;

                srcStage = vk::PipelineStageFlagBits::eTopOfPipe;
                dstStage = vk::PipelineStageFlagBits::eTransfer;
            } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal &&
                       newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                srcAccessMask |= vk::AccessFlagBits::eTransferWrite;
                dstAccessMask |= vk::AccessFlagBits::eShaderRead;

                srcStage = vk::PipelineStageFlagBits::eTransfer;
                dstStage = vk::PipelineStageFlagBits::eFragmentShader;
            } else {
                throw std::invalid_argument("unsupported layout transition!");
            }

            vk::ImageMemoryBarrier barrier(srcAccessMask,
                                           dstAccessMask,
                                           oldLayout,
                                           newLayout,
                                           VK_QUEUE_FAMILY_IGNORED,
                                           VK_QUEUE_FAMILY_IGNORED,
                                           image,
                                           vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

            cmdBuffer.pipelineBarrier(srcStage,
                                      dstStage,
                                      vk::DependencyFlags(),
                                      std::array<vk::MemoryBarrier, 0>{},
                                      std::array<vk::BufferMemoryBarrier, 0>{},
                                      std::array<vk::ImageMemoryBarrier, 1>{barrier});

            submit_one_time_buffer(device, cmdPool, queue, cmdBuffer);
        }

        void copy_buffer_to_image(vk::Device device,
                                  vk::CommandPool cmdPool,
                                  vk::Queue queue,
                                  const buffer& buffer,
                                  vk::Image image,
                                  core::extent<std::uint32_t> extent) {
            const auto cmdBuffer = create_one_time_buffer(device, cmdPool);

            vk::BufferImageCopy copyInfo(0,
                                         0,
                                         0,
                                         vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
                                         {0, 0, 0},
                                         {extent.width, extent.height, 1});

            cmdBuffer.copyBufferToImage(buffer.get_handle(), image, vk::ImageLayout::eTransferDstOptimal, copyInfo);

            submit_one_time_buffer(device, cmdPool, queue, cmdBuffer);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst