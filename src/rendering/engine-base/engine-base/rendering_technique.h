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

        enum class compare_operation {
            never,
            less,
            equal,
            less_or_equal,
            greater,
            not_equal,
            greater_or_equal,
            always
        };

        struct depth_settings {
            bool depthTestEnable;
            bool depthWriteEnable;
            compare_operation compareOperation;
            bool depthBoundsTestEnable;
            float minDepthBounds;
            float maxDepthBounds;

            bool operator==(const depth_settings& other) const noexcept;
        };

        struct stencil_settings {
            enum class stencil_operation {
                keep,
                zero,
                replace,
                increment_and_clamp,
                decrement_and_clamp,
                invert,
                increment_and_wrap,
                decrement_and_wrap
            };
            struct operation_state {
                stencil_operation failOperation;
                stencil_operation passOperation;
                stencil_operation depthFailOperation;
                compare_operation compareOperation;
                std::uint32_t compareMask;
                std::uint32_t writeMask;
                std::uint32_t reference;

                bool operator==(const operation_state& other) const noexcept;
            };

            bool enable;
            operation_state frontOperation;
            operation_state backOperation;

            bool operator==(const stencil_settings& other) const noexcept;
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

        enum class callback_mode { absolute, relative };

        using viewport_callback = std::function<viewport_settings(core::extent<std::uint32_t>)>;
        using scissor_callback = std::function<core::rectangle<std::int32_t, std::uint32_t>(core::extent<std::uint32_t>)>;

        struct draw_settings {
            viewport_callback viewportCallback;
            scissor_callback scissorCallback;
            depth_settings depthSettings;
            stencil_settings stencilSettings;
            std::vector<color_blending_settings> framebufferColorBlending;
            global_blending_settings globalColorBlending;
        };

        class rendering_technique {
        public:
            rendering_technique(std::string&& techniqueName,
                                viewport_callback&& viewportCallback,
                                scissor_callback&& scissorCallback,
                                const depth_settings& depthSettings,
                                const stencil_settings& stencilSettings,
                                std::vector<color_blending_settings>&& framebufferBlending,
                                const global_blending_settings& globalBlending,
                                core::extent<std::uint32_t> windowSize);
            rendering_technique(std::string&& techniqueName,
                                draw_settings&& drawSettings,
                                core::extent<std::uint32_t> windowSize);

            const std::string& get_name() const;

        protected:
            std::string m_techniqueName;
            viewport_callback m_viewportSettingsCallback;
            scissor_callback m_scissorCallback;
            viewport_settings m_viewportSettings;
            core::rectangle<std::int32_t, std::uint32_t> m_scissor;
            depth_settings m_depthSettings;
            stencil_settings m_stencilSettings;
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
        std::size_t seed = 0;
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
struct hash<tst::engine::base::depth_settings> {
    std::size_t operator()(const tst::engine::base::depth_settings& settings) const {
        std::size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.depthTestEnable));
        tst::hash_combine(seed, hasher(settings.depthWriteEnable));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.compareOperation)));
        tst::hash_combine(seed, hasher(settings.depthBoundsTestEnable));
        tst::hash_combine(seed, std::hash<float>{}(settings.minDepthBounds));
        tst::hash_combine(seed, std::hash<float>{}(settings.maxDepthBounds));
        return seed;
    }
};

template<>
struct hash<tst::engine::base::stencil_settings::operation_state> {
    std::size_t operator()(const tst::engine::base::stencil_settings::operation_state& settings) const {
        std::size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.failOperation)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.passOperation)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.depthFailOperation)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.compareOperation)));
        tst::hash_combine(seed, hasher(settings.compareMask));
        tst::hash_combine(seed, hasher(settings.writeMask));
        tst::hash_combine(seed, hasher(settings.reference));
        return seed;
    }
};

template<>
struct hash<tst::engine::base::stencil_settings> {
    std::size_t operator()(const tst::engine::base::stencil_settings& settings) const {
        std::size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.enable));
        tst::hash_combine(seed,
                          std::hash<tst::engine::base::stencil_settings::operation_state>()(settings.frontOperation));
        tst::hash_combine(seed,
                          std::hash<tst::engine::base::stencil_settings::operation_state>()(settings.backOperation));

        return seed;
    }
};

template<>
struct hash<tst::engine::base::color_blending_settings> {
    std::size_t operator()(const tst::engine::base::color_blending_settings& settings) const {
        std::size_t seed = 0;
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
        std::size_t seed = 0;
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
