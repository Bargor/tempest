// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <cstdint>
#include <util/hash.h>
#include <utility>

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
            enum class sample_count { samples_1 = 1, samples_2 = 2, samples_4 = 4, samples_8 = 8, samples_16 = 16 };

            bool enable;
            sample_count samples;

            bool operator==(const multisampling_settings& other) const noexcept;
        };

        //TODO: Make this struct
        class settings {
        public:
            constexpr static settings get_default_settings();

        public:
            constexpr settings(const rasterizer_settings& rasterizerSettings,
                               const multisampling_settings& multisamplingSettings);

        public:
            rasterizer_settings m_rasterizer;
            multisampling_settings m_multisampling;
        };

        constexpr settings settings::get_default_settings() {
            return base::settings{
                base::rasterizer_settings{false,
                                          false,
                                          base::rasterizer_settings::polygon_mode::fill,
                                          base::rasterizer_settings::cull_mode::back,
                                          base::rasterizer_settings::front_face::counter_clockwise,
                                          1.0f,
                                          {false, 0.0f, 0.0f, 0.0f}},
                base::multisampling_settings{false, base::multisampling_settings::sample_count::samples_1}};
        }

        constexpr settings::settings(const rasterizer_settings& rasterizerSettings,
                                     const multisampling_settings& multisamplingSettings)
            : m_rasterizer(rasterizerSettings), m_multisampling(multisamplingSettings) {
        }

    } // namespace base
} // namespace engine
} // namespace tst

namespace std {
template<>
struct hash<tst::engine::base::rasterizer_settings::depth_bias_settings> {
    std::size_t operator()(const tst::engine::base::rasterizer_settings::depth_bias_settings& settings) const {
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
struct hash<tst::engine::base::rasterizer_settings> {
    std::size_t operator()(const tst::engine::base::rasterizer_settings& settings) const {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.depthClamp));
        tst::hash_combine(seed, hasher(settings.rasterizerDiscard));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.polygonMode)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.cullMode)));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.frontFace)));
        tst::hash_combine(seed, std::hash<float>{}(settings.lineWidth));
        tst::hash_combine(seed,
                          std::hash<tst::engine::base::rasterizer_settings::depth_bias_settings>{}(settings.depthBias));
        return seed;
    }
};

template<>
struct hash<tst::engine::base::multisampling_settings> {
    std::size_t operator()(const tst::engine::base::multisampling_settings& settings) const {
        size_t seed = 0;
        hash<std::int32_t> hasher;
        tst::hash_combine(seed, hasher(settings.enable));
        tst::hash_combine(seed, hasher(static_cast<std::int32_t>(settings.samples)));
        return seed;
    }
};
} // namespace std
