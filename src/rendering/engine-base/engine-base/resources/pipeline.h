// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <array>
#include <common/rectangle.h>
#include <vector>

namespace tst {
namespace engine {
    namespace base {

        class pipeline {
        public:
            struct viewport {
                std::int32_t x;
                std::int32_t y;
                std::int32_t width;
                std::int32_t height;
                float minDepth;
                float maxDepth;
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
                };

                bool depthClamp;
                bool rasterizerDiscard;
                polygon_mode polygonMode;
                cull_mode cullMode;
                front_face frontFace;
                float lineWidth;
                depth_bias_settings depthBias;
            };

            struct multisampling_settings {
                enum class sample_count { samples_1, samples_2, samples_4, samples_8, samples_16 };

                bool enable;
                sample_count samples;
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
                };

                bool enable;
                blend_operation colorBlendOperation;
                blend_factor srcColorBlendFactor;
                blend_factor dstColorBlendFactor;
                blend_operation alphaBlendOperation;
                blend_factor srcAlphaBlendFactor;
                blend_factor dstAlphaBlendFactor;
                color_component_flags colorWriteMask;
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
            };

        public:
            viewport viewport;
            core::rectangle<std::int32_t, std::uint32_t> scissor;
            rasterizer_settings rasterizer;
            multisampling_settings multisampling;
            std::vector<color_blending_settings> framebufferColorBlending;
            global_blending_settings globalColorBlending;
        };

    } // namespace base
} // namespace engine
} // namespace tst
