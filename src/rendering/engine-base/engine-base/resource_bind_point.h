// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <cstdint>

namespace tst {
namespace engine {
    namespace base {
        enum class resource_bind_point : std::uint8_t {
            global_static,
            global_dynamic,
            frame_static,
            frame_dynamic,
            material_static,
            material_dynamic,
            object_static,
            object_dynamic,
            enum_size = 8
        };

        constexpr std::uint32_t resource_bind_points_count = static_cast<std::uint32_t>(resource_bind_point::enum_size);
    } // namespace base
} // namespace engine
} // namespace tst