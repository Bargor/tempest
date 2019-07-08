// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_format.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vertex_format::vertex_format()
            : m_bindingDescription(vk::VertexInputBindingDescription(0, sizeof(vertex), vk::VertexInputRate::eVertex))
            , m_attributeDescriptions({{{0, 0, vk::Format::eR32G32Sfloat, offsetof(vertex, pos)},
                                        {1, 0, vk::Format::eR32G32B32Sfloat, offsetof(vertex, color)}}}) {
        }

        const vk::VertexInputBindingDescription& vertex_format::get_binding_description() const {
            return m_bindingDescription;
        }

        const std::array<vk::VertexInputAttributeDescription, 2>& vertex_format::get_attribute_descriptions() const {
            return m_attributeDescriptions;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst