// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

namespace tst {
namespace engine {
    namespace base {

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

        class settings {
        public:
            settings();
            ~settings();

        private:
            rasterizer_settings m_rasterizer;
            multisampling_settings m_multisampling;
        };

    } // namespace base
} // namespace engine
} // namespace tst
