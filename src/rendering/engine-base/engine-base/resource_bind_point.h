// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <cstdint>
#include <util/flags.h>

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

        enum class bind_flag_bits : std::uint16_t {
            ePipeline = 0x0001,
            eGlobal_dynamic = 0x0002,
            eView_static = 0x0004,
            eView_dynamic = 0x0008,
            eMaterial_static = 0x0010,
            eMaterial_dynamic = 0x0020,
            eObject_static = 0x0040,
            eObject_dynamic = 0x0080,
            eDescriptor_sets = 0x00FE,
            eAll = 0xFFFF
        };

        using descritptor_bind_flags = core::flags<bind_flag_bits>;

        constexpr std::uint32_t resource_bind_points_count = static_cast<std::uint32_t>(resource_bind_point::enum_size);
    } // namespace base
} // namespace engine

namespace core {
    template<>
    struct flag_traits<engine::base::bind_flag_bits> {
        enum { eAll_flags = 0xFFFF };
    };
}
} // namespace tst