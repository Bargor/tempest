// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <array>
#include <common/rectangle.h>
#include <core.h>
#include <string>
#include <vector>

namespace tst {
namespace engine {
    namespace base {

        struct viewport_settings {
            std::int32_t x;
            std::int32_t y;
            std::int32_t width;
            std::int32_t height;
            float minDepth;
            float maxDepth;

            bool operator==(const viewport_settings& other) const noexcept;
        };

        struct color_blending_settings {
            enum class blend_operation {
                add,
                subtract,
                reverse_subtract,
                min,
                max,
            };

            enum class blend_factor {
                zero,
                one,
                src_color,
                one_minus_src_color,
                dst_color,
                one_minus_dst_color,
                src_alpha,
                one_minus_src_alpha,
                dst_alpha,
                one_minus_dst_alpha,
                constant_color,
                one_minus_constant_color,
                constant_alpha,
                one_minus_constant_alpha,
                src_alpha_saturate,
                src_1_color,
                one_minus_src_1_color,
                src_1_alpha,
                one_minus_src_1_alpha
            };

            struct color_component_flags {
                bool r;
                bool g;
                bool b;
                bool a;

                bool operator==(const color_component_flags& other) const noexcept;
            };

            bool enable;
            blend_operation colorBlendOperation;
            blend_factor srcColorBlendFactor;
            blend_factor dstColorBlendFactor;
            blend_operation alphaBlendOperation;
            blend_factor srcAlphaBlendFactor;
            blend_factor dstAlphaBlendFactor;
            color_component_flags colorWriteMask;

            bool operator==(const color_blending_settings& other) const noexcept;
        };

        struct global_blending_settings {
            enum class logic_operation {
                clear,
                and_op,
                and_reverse,
                copy,
                and_inverted,
                no_op,
                xor_op,
                or_op,
                nor_op,
                equivalent,
                invert,
                or_reverse,
                copy_inverted,
                or_inverted,
                nand_op,
                set
            };

            bool enable;
            logic_operation logicalOperation;
            std::array<float, 4> blendConstants;

            bool operator==(const global_blending_settings& other) const noexcept;
        };

        class rendering_technique {
        public:
            rendering_technique(const std::string& techniqueName,
                                const viewport_settings& viewport,
                                const core::rectangle<std::int32_t, std::uint32_t> scissor,
                                std::initializer_list<color_blending_settings> framebufferBlending,
                                const global_blending_settings& globalBlending);

            const std::string& get_name() const;

        protected:
            std::string m_techniqueName;
            viewport_settings m_viewportSettings;
            core::rectangle<std::int32_t, std::uint32_t> m_scissor;
            std::vector<color_blending_settings> m_framebufferColorBlending;
            global_blending_settings m_globalColorBlending;
        };

        TST_INLINE const std::string& rendering_technique::get_name() const {
            return m_techniqueName;
        }
    } // namespace base
} // namespace engine
} // namespace tst

namespace std {

template<>
struct hash<tst::engine::base::viewport_settings> {
    std::size_t operator()(const tst::engine::base::viewport_settings& settings) const {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.x));
        tst::hash_combine(seed, hasher(settings.y));
        tst::hash_combine(seed, hasher(settings.width));
        tst::hash_combine(seed, hasher(settings.height));
        tst::hash_combine(seed, std::hash<float>{}(settings.minDepth));
        tst::hash_combine(seed, std::hash<float>{}(settings.maxDepth));
        return seed;
    }
};

template<>
struct hash<tst::engine::base::color_blending_settings::color_component_flags> {
    std::size_t operator()(const tst::engine::base::color_blending_settings::color_component_flags& settings) {
        std::size_t hash = static_cast<std::uint64_t>(settings.r) << 24 | static_cast<std::uint64_t>(settings.g) << 16 |
            static_cast<std::uint64_t>(settings.b) << 8 | static_cast<std::uint64_t>(settings.a);

        return hash;
    }
};

template<>
struct hash<tst::engine::base::color_blending_settings> {
    std::size_t operator()(const tst::engine::base::color_blending_settings& settings) const {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.enable));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.colorBlendOperation)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.srcColorBlendFactor)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.dstColorBlendFactor)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.alphaBlendOperation)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.srcAlphaBlendFactor)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.dstAlphaBlendFactor)));
        tst::hash_combine(
            seed,
            std::hash<tst::engine::base::color_blending_settings::color_component_flags>{}(settings.colorWriteMask));
        return seed;
    }
};

template<>
struct hash<tst::engine::base::global_blending_settings> {
    std::size_t operator()(const tst::engine::base::global_blending_settings& settings) const {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.enable));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.logicalOperation)));
        tst::hash_combine(seed, std::hash<float>{}(settings.blendConstants[0]));
        tst::hash_combine(seed, std::hash<float>{}(settings.blendConstants[1]));
        tst::hash_combine(seed, std::hash<float>{}(settings.blendConstants[2]));
        tst::hash_combine(seed, std::hash<float>{}(settings.blendConstants[3]));

        return seed;
    }
};

} // namespace std
