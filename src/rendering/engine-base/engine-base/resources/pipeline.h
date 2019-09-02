// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <array>
#include <common/rectangle.h>
#include <util/hash.h>
#include <vector>

namespace tst {
namespace engine {
    namespace base {

        class pipeline {
        public:
            struct viewport_settings {
                std::int32_t x;
                std::int32_t y;
                std::int32_t width;
                std::int32_t height;
                float minDepth;
                float maxDepth;

                bool operator==(const viewport_settings& other) const noexcept;
            };

            struct rasterizer_settings {
                enum class polygon_mode { point, line, fill };
                enum class cull_mode { front, back, front_and_back };
                enum class front_face { clockwise, counter_clockwise };
                struct depth_bias_settings {
                    bool enable;
                    float constantFactor;
                    float clamp;
                    float slopeFactor;

                    bool operator==(const depth_bias_settings& other) const noexcept;
                };

                bool depthClamp;
                bool rasterizerDiscard;
                polygon_mode polygonMode;
                cull_mode cullMode;
                front_face frontFace;
                float lineWidth;
                depth_bias_settings depthBias;

                bool operator==(const rasterizer_settings& other) const noexcept;
            };

            struct multisampling_settings {
                enum class sample_count { samples_1, samples_2, samples_4, samples_8, samples_16 };

                bool enable;
                sample_count samples;

                bool operator==(const multisampling_settings& other) const noexcept;
            };

            struct color_blending_settings {
                enum class blend_operation {
                    add,
                    subtract,
                    eReverseSubtract,
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
                    nor,
                    equivalent,
                    invert,
                    or_reverse,
                    copy_inverted,
                    or_inverted,
                    nand,
                    set
                };

                bool enable;
                logic_operation logicalOperation;
                std::array<float, 4> blendConstants;

                bool operator==(const global_blending_settings& other) const noexcept;
            };

            bool operator==(const pipeline& other) const noexcept;

        public:
            viewport_settings viewport;
            core::rectangle<std::int32_t, std::uint32_t> scissor;
            rasterizer_settings rasterizer;
            multisampling_settings multisampling;
            std::vector<color_blending_settings> framebufferColorBlending;
            global_blending_settings globalColorBlending;
        };

    } // namespace base
} // namespace engine
} // namespace tst

namespace std {

template<>
struct hash<tst::engine::base::pipeline::viewport_settings> {
    std::size_t operator()(const tst::engine::base::pipeline::viewport_settings& settings) {
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
struct hash<tst::engine::base::pipeline::rasterizer_settings::depth_bias_settings> {
    std::size_t operator()(const tst::engine::base::pipeline::rasterizer_settings::depth_bias_settings& settings) {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.enable));
        tst::hash_combine(seed, std::hash<float>{}(settings.constantFactor));
        tst::hash_combine(seed, std::hash<float>{}(settings.clamp));
        tst::hash_combine(seed, std::hash<float>{}(settings.slopeFactor));

        return seed;
    }
};

template<>
struct hash<tst::engine::base::pipeline::rasterizer_settings> {
    std::size_t operator()(const tst::engine::base::pipeline::rasterizer_settings& settings) {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.depthClamp));
        tst::hash_combine(seed, hasher(settings.rasterizerDiscard));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.polygonMode)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.cullMode)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.frontFace)));
        tst::hash_combine(seed, std::hash<float>{}(settings.lineWidth));
        tst::hash_combine(
            seed, std::hash<tst::engine::base::pipeline::rasterizer_settings::depth_bias_settings>{}(settings.depthBias));
        return seed;
    }
};

template<>
struct hash<tst::engine::base::pipeline::multisampling_settings> {
    std::size_t operator()(const tst::engine::base::pipeline::multisampling_settings& settings) {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.enable));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.samples)));
        return seed;
    }
};

template<>
struct hash<tst::engine::base::pipeline::color_blending_settings::color_component_flags> {
    std::size_t operator()(const tst::engine::base::pipeline::color_blending_settings::color_component_flags& settings) {
        std::size_t hash = static_cast<std::int32_t>(settings.r) << 24 | static_cast<std::int32_t>(settings.g) << 16 |
            static_cast<std::int32_t>(settings.b) << 8 | static_cast<std::int32_t>(settings.a);

        return hash;
    }
};

template<>
struct hash<tst::engine::base::pipeline::color_blending_settings> {
    std::size_t operator()(const tst::engine::base::pipeline::color_blending_settings& settings) {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.enable));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.colorBlendOperation)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.srcColorBlendFactor)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.dstColorBlendFactor)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.alphaBlendOperation)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.srcAlphaBlendFactor)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.dstAlphaBlendFactor)));
        tst::hash_combine(seed,
                          std::hash<tst::engine::base::pipeline::color_blending_settings::color_component_flags>{}(
                              settings.colorWriteMask));
        return seed;
    }
};

template<>
struct hash<tst::engine::base::pipeline::global_blending_settings> {
    std::size_t operator()(const tst::engine::base::pipeline::global_blending_settings& settings) {
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

template<>
struct hash<tst::engine::base::pipeline> {
    std::size_t operator()(const tst::engine::base::pipeline& pipeline) {
        size_t seed = 0;

        tst::hash_combine(seed, std::hash<tst::engine::base::pipeline::viewport_settings>{}(pipeline.viewport));
        tst::hash_combine(seed, std::hash<tst::core::rectangle<std::int32_t, std::uint32_t>>{}(pipeline.scissor));
        tst::hash_combine(seed, std::hash<tst::engine::base::pipeline::rasterizer_settings>{}(pipeline.rasterizer));
        tst::hash_combine(seed, std::hash<tst::engine::base::pipeline::multisampling_settings>{}(pipeline.multisampling));
        for (auto& colorBlending : pipeline.framebufferColorBlending) {
            tst::hash_combine(seed, std::hash<tst::engine::base::pipeline::color_blending_settings>{}(colorBlending));
        }
        tst::hash_combine(
            seed, std::hash<tst::engine::base::pipeline::global_blending_settings>{}(pipeline.globalColorBlending));

        return seed;
    }
};

} // namespace std
