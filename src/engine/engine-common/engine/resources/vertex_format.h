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
            uint,
            uint2,
            uint3,
            uint4
        };

        enum class location { position, normal, texcoord, binormal };

        struct attribute_descriptor {
            vertex_format::location location;
            vertex_format::format format;
            std::uint32_t offset;
            std::uint32_t stride;
            std::uint32_t divisor;
        };

    public:
        vertex_format();

        void add_attribute(
            location location, format format, std::uint32_t offset, std::uint32_t stride, std::uint32_t divisor);
        const std::vector<attribute_descriptor>& get_atttibutes() const noexcept;

    private:
        std::vector<attribute_descriptor> m_attributes;
    };
} // namespace engine
} // namespace tst