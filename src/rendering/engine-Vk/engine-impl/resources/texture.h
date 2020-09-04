// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "settings.h"

#include <application/image_data.h>
#include <cstddef>
#include <engine-base/resource_bind_point.h>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        struct buffer_construction_info;
        class resource_cache;

        using descriptor_set = std::array<vk::DescriptorSet, settings::m_inFlightFrames>;

        class texture {
        public:
            texture(const buffer_construction_info& info,
                    const resource_cache& resourceCache,
                    vk::BufferUsageFlags flags,
                    vk::MemoryPropertyFlags memoryFlags,
                    const application::image_data& imageData,
                    const std::uint32_t& resourceIndex,
                    vk::Sampler = nullptr);
            ~texture();

            texture(texture&& other) noexcept;

        public:
            void bind_texture(const std::string& shaderName, base::resource_bind_point bindPoint, std::uint32_t binding);

        private:
            vk::Device m_logicalDevice;
            const resource_cache& m_resourceCache;
            vk::Image m_textureImage;
            vk::DeviceMemory m_textureMemory;
            vk::ImageView m_textureView;
            vk::Sampler m_sampler;
            const std::uint32_t& m_resourceIndex;
            const descriptor_set* m_descriptorSets;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
