// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_format {
        public:
            vertex_format();

            const vk::VertexInputBindingDescription& get_binding_description() const;
            const std::array<vk::VertexInputAttributeDescription, 2>& get_attribute_descriptions() const;

        private:
            vk::VertexInputBindingDescription m_bindingDescription;
            std::array<vk::VertexInputAttributeDescription, 2> m_attributeDescriptions;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
