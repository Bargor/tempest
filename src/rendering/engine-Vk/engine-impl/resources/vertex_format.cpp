// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_format.h"

namespace tst {
namespace engine {
    namespace vulkan {

        vk::Format translate_format(base::vertex_format::format format) {
            switch (format) {
            case base::vertex_format::format::invalid:
                return vk::Format::eUndefined;
            case base::vertex_format::format::float1:
                return vk::Format::eR32Sfloat;
            case base::vertex_format::format::float2:
                return vk::Format::eR32G32Sfloat;
            case base::vertex_format::format::float3:
                return vk::Format::eR32G32B32Sfloat;
            case base::vertex_format::format::float4:
                return vk::Format::eR32G32B32A32Sfloat;
            case base::vertex_format::format::int1:
                return vk::Format::eR32Sint;
            case base::vertex_format::format::int2:
                return vk::Format::eR32G32Sint;
            case base::vertex_format::format::int3:
                return vk::Format::eR32G32B32Sint;
            case base::vertex_format::format::int4:
                return vk::Format::eR32G32B32A32Sint;
            case base::vertex_format::format::uint1:
                return vk::Format::eR32Uint;
            case base::vertex_format::format::uint2:
                return vk::Format::eR32G32Uint;
            case base::vertex_format::format::uint3:
                return vk::Format::eR32G32B32Uint;
            case base::vertex_format::format::uint4:
                return vk::Format::eR32G32B32A32Uint;
            }
            assert(false);
            return vk::Format::eUndefined;
        }

        vk::VertexInputBindingDescription
        translate_binding_description(const std::vector<base::vertex_format::attribute_descriptor>& attributes) {
            return vk::VertexInputBindingDescription{0,
                                                     attributes[0].stride,
                                                     attributes[0].divisor == 0 ? vk::VertexInputRate::eVertex :
                                                                                  vk::VertexInputRate::eInstance};
        }

        std::vector<vk::VertexInputAttributeDescription>
        translate_attribute_descriptions(const std::vector<base::vertex_format::attribute_descriptor>& attributes) {
            std::vector<vk::VertexInputAttributeDescription> descriptions;
            descriptions.reserve(attributes.size());

            for (auto& attribute : attributes) {
                descriptions.emplace_back(vk::VertexInputAttributeDescription{
                    static_cast<std::uint32_t>(attribute.location), 0, translate_format(attribute.format), attribute.offset});
            }
            return descriptions;
        }

        vertex_format::vertex_format(base::vertex_format::primitive_topology topology) : base::vertex_format(topology) {
        }

        vk::VertexInputBindingDescription vertex_format::get_binding_description() const noexcept {
            return translate_binding_description(m_attributes);
        }

        std::vector<vk::VertexInputAttributeDescription> vertex_format::get_attribute_descriptions() const noexcept {
            return translate_attribute_descriptions(m_attributes);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
