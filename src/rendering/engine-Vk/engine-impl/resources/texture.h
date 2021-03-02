// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"
#include "settings.h"

#include <application/image_data.h>
#include <cstddef>
#include <engine-base/resource_bind_point.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class resource_cache;

        using descriptor_set = std::array<vk::DescriptorSet, settings::m_inFlightFrames>;

        class texture {
        public:
            struct creation_info {
                buffer::creation_info bufferCreationInfo;
                const resource_cache& resourceCache;
                vk::BufferUsageFlags flags;
                vk::MemoryPropertyFlags memoryFlags;
                application::image_data imageData;
                const std::uint32_t& resourceIndex;
                vk::Sampler sampler = nullptr;
            };

        public:
            texture(const creation_info& info);
            ~texture();

            texture(texture&& other) noexcept;

            texture& operator=(texture&& other);

        public:
            void bind_texture(const std::string& shaderName, base::resource_bind_point bindPoint, std::uint32_t binding);

        private:
            vk::Device m_logicalDevice;
            std::reference_wrapper<const resource_cache> m_resourceCache;
            vk::Image m_textureImage;
            vk::DeviceMemory m_textureMemory;
            vk::ImageView m_textureView;
            vk::Sampler m_sampler;
            std::reference_wrapper<const std::uint32_t> m_resourceIndex;
            const descriptor_set* m_descriptorSets;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
