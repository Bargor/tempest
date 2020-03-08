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

        class rendering_technique;
        class settings;

        struct pipeline_settings {
        public:
            friend struct std::hash<pipeline_settings>;

            pipeline_settings(const viewport_settings& viewportSettings,
                              const core::rectangle<std::int32_t, std::uint32_t>& scissor,
                              const depth_settings& depthSettings,
                              const stencil_settings& stencilSettings,
                              const rasterizer_settings& rasterizerSettings,
                              const multisampling_settings& multisamplingSettings,
                              const std::vector<color_blending_settings>& blendingSettings,
                              const global_blending_settings& globalBlendingSettings);

            bool operator==(const pipeline_settings& other) const noexcept;

        public:
            viewport_settings m_viewport;
            core::rectangle<std::int32_t, std::uint32_t> m_scissor;
            depth_settings m_depthSettings;
            stencil_settings m_stencilSettings;
            rasterizer_settings m_rasterizer;
            multisampling_settings m_multisampling;
            std::vector<color_blending_settings> m_framebufferColorBlending;
            global_blending_settings m_globalColorBlending;
        };

    } // namespace base
} // namespace engine
} // namespace tst

namespace std {

template<>
struct hash<tst::engine::base::pipeline_settings> {
    std::size_t operator()(const tst::engine::base::pipeline_settings& pipeline) const {
        size_t seed = 0;

        tst::hash_combine(seed, std::hash<tst::engine::base::viewport_settings>{}(pipeline.m_viewport));
        tst::hash_combine(seed, std::hash<tst::core::rectangle<std::int32_t, std::uint32_t>>{}(pipeline.m_scissor));
        tst::hash_combine(seed, std::hash<tst::engine::base::rasterizer_settings>{}(pipeline.m_rasterizer));
        tst::hash_combine(seed, std::hash<tst::engine::base::multisampling_settings>{}(pipeline.m_multisampling));
        for (auto& colorBlending : pipeline.m_framebufferColorBlending) {
            tst::hash_combine(seed, std::hash<tst::engine::base::color_blending_settings>{}(colorBlending));
        }
        tst::hash_combine(seed, std::hash<tst::engine::base::global_blending_settings>{}(pipeline.m_globalColorBlending));

        return seed;
    }
};

} // namespace std
