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

            vertex_format(const base::vertex_format& vertex_format);

            const std::vector<vk::VertexInputBindingDescription>& get_binding_descriptions() const noexcept;
            const std::vector<vk::VertexInputAttributeDescription>& get_attribute_descriptions() const noexcept;

        private:
            std::vector<vk::VertexInputBindingDescription> m_bindingDescriptions;
            std::vector<vk::VertexInputAttributeDescription> m_attributeDescriptions;
        };

        TST_INLINE const std::vector<vk::VertexInputBindingDescription>& vertex_format::get_binding_descriptions() const
            noexcept {
            return m_bindingDescriptions;
        }

        TST_INLINE const std::vector<vk::VertexInputAttributeDescription>& vertex_format::get_attribute_descriptions() const
            noexcept {
            return m_attributeDescriptions;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
