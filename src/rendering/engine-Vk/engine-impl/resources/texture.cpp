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
                         const application::image_data& imageData)
            : m_logicalDevice(logicalDevice) {
            buffer stagingBuffer(
                logicalDevice, queueHandle, cmdPool, imageData.memorySize, flags, memoryProperties, memoryFlags);

            std::tie(m_textureImage, m_textureMemory) =
                create_image(logicalDevice,
                             {imageData.imageSize.width, imageData.imageSize.height},
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
            copy_buffer_to_image(logicalDevice, cmdPool, queueHandle, stagingBuffer, m_textureImage, imageData.imageSize);
            transition_image_layout(logicalDevice,
                                    queueHandle,
                                    cmdPool,
                                    m_textureImage,
                                    vk::Format::eR8G8B8Srgb,
                                    vk::ImageLayout::eTransferDstOptimal,
                                    vk::ImageLayout::eShaderReadOnlyOptimal);

            m_textureView = create_image_view(
                logicalDevice, m_textureImage, vk::Format::eR8G8B8Srgb, vk::ImageAspectFlagBits::eColor);
        }

        texture::~texture() {
            m_logicalDevice.destroyImageView(m_textureView);
            m_logicalDevice.destroyImage(m_textureImage);
            m_logicalDevice.freeMemory(m_textureMemory);
        }

        texture::texture(texture&& other) noexcept
            : m_logicalDevice(other.m_logicalDevice)
            , m_textureImage(other.m_textureImage)
            , m_textureMemory(other.m_textureMemory)
            , m_textureView(other.m_textureView) {
            other.m_textureImage = nullptr;
            other.m_textureMemory = nullptr;
            other.m_textureView = nullptr;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
