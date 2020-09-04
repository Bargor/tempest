// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "glm.h"

#include <util/hash.h>
#include <vector>

namespace tst {
namespace engine {

    struct vertex {
        glm::vec3 pos;
        glm::vec3 color;
        glm::vec2 texCoord;
    };

    TST_INLINE bool operator==(const vertex& rhs, const vertex& lhs) {
        return rhs.pos == lhs.pos && rhs.color == lhs.color && rhs.texCoord == lhs.texCoord;
    }

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

            enum class location { position, normal, texcoord, binormal, color };

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

namespace std {
template<>
struct hash<tst::engine::vertex> {
    size_t operator()(tst::engine::vertex const& vertex) const {

        size_t seed = 0;
        tst::hash_combine(seed, hash<glm::vec3>{}(vertex.pos));
        tst::hash_combine(seed, hash<glm::vec3>{}(vertex.color));
        tst::hash_combine(seed, hash<glm::vec2>{}(vertex.texCoord));

        return seed;
    }
};
} // namespace std
