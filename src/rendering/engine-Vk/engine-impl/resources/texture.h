// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <common/rectangle.h>
#include <cstddef>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class texture {
        public:
            texture(vk::Device logicalDevice,
                    vk::Queue queueHandle,
                    vk::CommandPool cmdPool,
                    vk::BufferUsageFlags flags,
                    const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                    vk::MemoryPropertyFlags memoryFlags,
                    std::size_t bytesCount,
                    core::extent<std::uint32_t> imageSize,
                    void* imageData);
            ~texture();

            texture(texture&& other) noexcept;

        private:
            vk::Device m_logicalDevice;
            vk::Image m_textureImage;
            vk::DeviceMemory m_textureMemory;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
