// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "texture.h"
#include "uniform_buffer.h"

#include <engine-base/material.h>
#include <optional>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {
        class material : base::material {
            using super = base::material;

        public:
            struct creation_info {
                std::vector<texture::creation_info> textureInfos;
                const descriptor_set& staticDescriptorSets;
                std::optional<uniform_buffer::creation_info> staticUniformInfo;
                std::optional<uniform_buffer::creation_info> dynamicUniformInfo;
                std::uint32_t staticStorageSize;
                std::uint32_t dynamicStorageSize;
                const std::uint32_t& resourceIndex;
            };

        public:
            material(std::string&& name, const std::string& shaderName, const creation_info& info);

            material(std::string&& name,
                     std::vector<texture>&& textures,
                     const descriptor_set& staticDescriptorSets,
                     const std::uint32_t& resourceIndex);
            material(std::string&& name,
                     uniform_buffer&& staticUniforms,
                     uniform_buffer&& dynamicUniforms,
                     std::vector<texture>&& textures,
                     const descriptor_set& staticDescriptorSets,
                     const std::uint32_t& resourceIndex);

            material(material&& other) noexcept;

            material& operator=(material&& other);

        public:
            vk::DescriptorSet get_static_descriptor_set() const noexcept;

        private:
            std::vector<texture> m_textures;
            std::optional<uniform_buffer> m_staticUniformBuffer;
            std::optional<uniform_buffer> m_dynamicUniformBuffer;
            std::reference_wrapper<const descriptor_set> m_staticDescriptorSets;
            std::reference_wrapper<const std::uint32_t> m_resourceIndex;
        };

        TST_INLINE vk::DescriptorSet material::get_static_descriptor_set() const noexcept {
            return m_staticDescriptorSets.get()[m_resourceIndex];
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
