// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <cstdint>

namespace tst {
namespace engine {
    namespace base {
        enum class shader_type : std::uint8_t {
            vertex,
            fragment,
            geometry,
            compute,
            tessellation_control,
            tessellation_evaluation,
            enum_size = 6
        };

        constexpr std::uint32_t shader_types_count = static_cast<std::uint32_t>(shader_type::enum_size);
    }
} // namespace engine
} // namespace tst