// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vector>

namespace tst {
namespace engine {

    struct vertex {
        glm::vec2 pos;
        glm::vec3 color;
    };

    namespace base {

        class vertex_format {
        public:
            enum class format {
                invalid,
                float1,
                float2,
                float3,
                float4,
                int1,
                int2,
                int3,
                int4,
                uint1,
                uint2,
                uint3,
                uint4
            };

            enum class location { position, normal, texcoord, binormal };

            enum class primitive_topology {
                point_list,
                line_list,
                line_strip,
                triangle_list,
                triangle_strip,
                triangle_fan
            };

            struct attribute_descriptor {
                vertex_format::location location;
                vertex_format::format format;
                std::uint32_t offset;
                std::uint32_t stride;
                std::uint32_t divisor;
            };

        public:
            vertex_format(primitive_topology topology);

            void add_attribute(
                location location, format format, std::uint32_t offset, std::uint32_t stride, std::uint32_t divisor);
            const std::vector<attribute_descriptor>& get_attribute_descriptors() const noexcept;
            primitive_topology get_topology() const noexcept;

        protected:
            std::vector<attribute_descriptor> m_attributes;
            primitive_topology m_topology;
        };

    } // namespace base
} // namespace engine
} // namespace tst
