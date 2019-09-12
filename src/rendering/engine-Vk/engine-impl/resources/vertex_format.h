// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <array>
#include <engine-base/vertex_format.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_format : public base::vertex_format {
        public:
            using super = base::vertex_format;

            vertex_format();

            const vk::VertexInputBindingDescription& get_binding_description() const noexcept;
            const std::array<vk::VertexInputAttributeDescription, 2>& get_attribute_descriptions() const noexcept;
            vk::PrimitiveTopology get_topology() const noexcept;

        private:
            vk::VertexInputBindingDescription m_bindingDescription;
            std::array<vk::VertexInputAttributeDescription, 2> m_attributeDescriptions;
            vk::PrimitiveTopology m_topology;
        };

        TST_INLINE const vk::VertexInputBindingDescription& vertex_format::get_binding_description() const noexcept {
            return m_bindingDescription;
        }

        TST_INLINE const std::array<vk::VertexInputAttributeDescription, 2>& vertex_format::get_attribute_descriptions() const
            noexcept {
            return m_attributeDescriptions;
        }

        TST_INLINE vk::PrimitiveTopology vertex_format::get_topology() const noexcept {
            return m_topology;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
