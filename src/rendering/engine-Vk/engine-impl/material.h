// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/texture.h"
#include "resources/uniform_buffer.h"

#include <concepts>
#include <engine-base/material.h>
#include <optional>
#include <type_traits>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {
        class material : base::material {
            using super = base::material;

        public:
            material::material(const std::string& name,
                               std::vector<texture>&& textures,
                               const descriptor_set& staticDescriptorSets,
                               const std::uint32_t& resourceIndex);
            material::material(const std::string& name,
                               uniform_buffer&& staticUniforms,
                               uniform_buffer&& dynamicUniforms,
                               std::vector<texture>&& textures,
                               const descriptor_set& staticDescriptorSets,
                               const std::uint32_t& resourceIndex);

        public:
            vk::DescriptorSet get_static_descriptor_set() const noexcept;
            vk::DescriptorSet get_dynamic_descriptor_set() const noexcept;

        private:
            std::vector<texture> m_textures;
            std::optional<uniform_buffer> m_staticUniformBuffer;
            std::optional<uniform_buffer> m_dynamicUniformBuffer;
            const std::uint32_t& m_resourceIndex;
            const std::array<vk::DescriptorSet, settings::m_inFlightFrames>& m_staticDescriptorSets;
        };

        TST_INLINE vk::DescriptorSet material::get_static_descriptor_set() const noexcept {
            return m_staticDescriptorSets[m_resourceIndex];
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
