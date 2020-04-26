// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "texture.h"

#include "buffer.h"
#include "image.h"

namespace tst {
namespace engine {
    namespace vulkan {

        texture::texture(vk::Device logicalDevice,
                         vk::Queue queueHandle,
                         vk::CommandPool cmdPool,
                         vk::BufferUsageFlags flags,
                         const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                         vk::MemoryPropertyFlags memoryFlags,
                         std::size_t bytesCount,
                         core::extent<std::uint32_t> imageSize,
                         void*)
            : m_logicalDevice(logicalDevice) {
            buffer stagingBuffer(logicalDevice, queueHandle, cmdPool, bytesCount, flags, memoryProperties, memoryFlags);

            std::tie(m_textureImage, m_textureMemory) =
                create_image(logicalDevice,
                             {imageSize.width, imageSize.height},
                             vk::Format::eR8G8B8A8Srgb,
                             vk::ImageTiling::eOptimal,
                             vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                             memoryProperties,
                             vk::MemoryPropertyFlagBits::eDeviceLocal);

            transition_image_layout(logicalDevice,
                                    queueHandle,
                                    cmdPool,
                                    m_textureImage,
                                    vk::Format::eR8G8B8Srgb,
                                    vk::ImageLayout::eUndefined,
                                    vk::ImageLayout::eTransferDstOptimal);
            copy_buffer_to_image(logicalDevice, cmdPool, queueHandle, stagingBuffer, m_textureImage, imageSize);
            transition_image_layout(logicalDevice,
                                    queueHandle,
                                    cmdPool,
                                    m_textureImage,
                                    vk::Format::eR8G8B8Srgb,
                                    vk::ImageLayout::eTransferDstOptimal,
                                    vk::ImageLayout::eShaderReadOnlyOptimal);
        }

        texture::~texture() {
            m_logicalDevice.destroyImage(m_textureImage);
            m_logicalDevice.freeMemory(m_textureMemory);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
