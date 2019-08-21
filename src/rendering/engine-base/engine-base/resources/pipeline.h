// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <common/rectangle.h>

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

            struct depth_bias_settings {
                bool enable;
                float constantFactor;
                float clamp;
                float slopeFactor;
            };

            struct rasterizer_settings {
                enum class polygon_mode { point, line, fill };
                enum class cull_mode { front, back, front_and_back };
                enum class front_face { clockwise, counter_clockwise };

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

            struct color_blending_settings {};

        public:
            viewport viewport;
            rectangle<std::int32_t, std::uint32_t> scissor;
            rasterizer_settings rasterizer;
            multisampling_settings multisampling;
            color_blending_settings colorBlending;
        };

    } // namespace base
} // namespace engine
} // namespace tst
