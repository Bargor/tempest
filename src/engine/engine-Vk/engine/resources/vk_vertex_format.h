// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <engine/resources/vertex_format.h>

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class vk_vertex_format {
        public:
            vk_vertex_format(const vertex_format& inputFormat);

            const vk::VertexInputBindingDescription& get_binding_description() const;
            const std::array<vk::VertexInputAttributeDescription, 2>& get_attribute_descriptions() const;

        private:
            vk::VertexInputBindingDescription m_bindingDescription;
            std::array<vk::VertexInputAttributeDescription, 2> m_attributeDescriptions;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
