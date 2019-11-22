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

            vertex_format(base::vertex_format::primitive_topology topology);

            std::vector<vk::VertexInputBindingDescription> get_binding_descriptions() const noexcept;
            std::vector<vk::VertexInputAttributeDescription> get_attribute_descriptions() const noexcept;

        private:
            std::vector<vk::VertexInputBindingDescription> m_bindingDescriptions;
            std::vector<vk::VertexInputAttributeDescription> m_attributeDescriptions;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
