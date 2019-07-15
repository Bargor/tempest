// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <array>
#include <vector>

namespace tst {
namespace engine {

    class vertex_format {
    public:
        enum class format { invalid, float, float2, float3, float4, int, int2, int3, int4, uint, uint2, uint3, uint4 };
        enum class location { position, normal, texcoord, binormal };

        struct attribute_descriptor {
            location location;
            format format;
            std::uint32_t divisor;
            std::uint32_t offset;
            std::uint32_t stride;
        };

        vertex_format();

    private:
        std::vector<attribute_descriptor> attributes;
    };
} // namespace engine
} // namespace tst