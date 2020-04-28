// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"

#include <application/image_data.h>
#include <cstddef>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class resource_cache;

        class texture {
        public:
            texture(vk::Device logicalDevice,
                    vk::Queue queueHandle,
                    vk::CommandPool cmdPool,
                    vk::DescriptorPool descPool,
                    const resource_cache& resourceCache,
                    vk::BufferUsageFlags flags,
                    const vk::PhysicalDeviceMemoryProperties& memoryProperties,
                    vk::MemoryPropertyFlags memoryFlags,
                    const application::image_data& imageData,
                    vk::Sampler = nullptr);
            ~texture();

            texture(texture&& other) noexcept;

        public:
            void bind_texture(const std::string& shaderName, shader_type type, std::uint32_t binding);

        public:
            vk::DescriptorSet get_descriptor_set() const noexcept;

        private:
            vk::Device m_logicalDevice;
            vk::DescriptorPool m_descPool;
            const resource_cache& m_resourceCache;
            vk::Image m_textureImage;
            vk::DeviceMemory m_textureMemory;
            vk::ImageView m_textureView;
            vk::Sampler m_sampler;
            vk::DescriptorSet m_descriptorSet;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
