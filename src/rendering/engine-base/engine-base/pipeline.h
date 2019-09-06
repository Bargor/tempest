// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "rendering_technique.h"
#include "settings.h"

#include <array>
#include <util/hash.h>
#include <vector>

namespace tst {
namespace engine {
    namespace base {

        class pipeline {
        public:
            friend struct std::hash<pipeline>;

            pipeline(const viewport_settings& viewportSettings,
                     const core::rectangle<std::int32_t, std::uint32_t>& scissor,
                     const rasterizer_settings& rasterizerSettings,
                     const multisampling_settings& multisamplingSettings,
                     const std::initializer_list<color_blending_settings> blendingSettings,
                     const global_blending_settings& globalBlendingSettings);

            bool operator==(const pipeline& other) const noexcept;

        private:
            viewport_settings m_viewport;
            core::rectangle<std::int32_t, std::uint32_t> m_scissor;
            rasterizer_settings m_rasterizer;
            multisampling_settings m_multisampling;
            global_blending_settings m_globalColorBlending;
        };

    } // namespace base
} // namespace engine
} // namespace tst

namespace std {

template<>
struct hash<tst::engine::base::pipeline::viewport_settings> {
    std::size_t operator()(const tst::engine::base::pipeline::viewport_settings& settings) const {
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
    std::size_t operator()(const tst::engine::base::pipeline::rasterizer_settings::depth_bias_settings& settings) const {
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
    std::size_t operator()(const tst::engine::base::pipeline::rasterizer_settings& settings) const {
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
    std::size_t operator()(const tst::engine::base::pipeline::multisampling_settings& settings) const {
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
        std::size_t hash = static_cast<std::uint64_t>(settings.r) << 24 | static_cast<std::uint64_t>(settings.g) << 16 |
            static_cast<std::uint64_t>(settings.b) << 8 | static_cast<std::uint64_t>(settings.a);

        return hash;
    }
};

template<>
struct hash<tst::engine::base::pipeline::color_blending_settings> {
    std::size_t operator()(const tst::engine::base::pipeline::color_blending_settings& settings) const {
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
    std::size_t operator()(const tst::engine::base::pipeline::global_blending_settings& settings) const {
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
    std::size_t operator()(const tst::engine::base::pipeline& pipeline) const {
        size_t seed = 0;

        tst::hash_combine(seed, std::hash<tst::engine::base::pipeline::viewport_settings>{}(pipeline.m_viewport));
        tst::hash_combine(seed, std::hash<tst::core::rectangle<std::int32_t, std::uint32_t>>{}(pipeline.m_scissor));
        tst::hash_combine(seed, std::hash<tst::engine::base::pipeline::rasterizer_settings>{}(pipeline.m_rasterizer));
        tst::hash_combine(seed,
                          std::hash<tst::engine::base::pipeline::multisampling_settings>{}(pipeline.m_multisampling));
        for (auto& colorBlending : pipeline.m_framebufferColorBlending) {
            tst::hash_combine(seed, std::hash<tst::engine::base::pipeline::color_blending_settings>{}(colorBlending));
        }
        tst::hash_combine(
            seed, std::hash<tst::engine::base::pipeline::global_blending_settings>{}(pipeline.m_globalColorBlending));

        return seed;
    }
};

} // namespace std
