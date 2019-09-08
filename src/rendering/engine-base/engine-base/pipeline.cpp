// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "pipeline.h"

namespace tst {
namespace engine {
    namespace base {     

        pipeline::pipeline(const viewport_settings& viewportSettings,
                           const core::rectangle<std::int32_t, std::uint32_t>& scissor,
                           const rasterizer_settings& rasterizerSettings,
                           const multisampling_settings& multisamplingSettings,
                           const std::initializer_list<color_blending_settings> blendingSettings,
                           const global_blending_settings& globalBlendingSettings)
            : m_viewport(viewportSettings)
            , m_scissor(scissor)
            , m_rasterizer(rasterizerSettings)
            , m_multisampling(multisamplingSettings)
            , m_framebufferColorBlending(blendingSettings)
            , m_globalColorBlending(globalBlendingSettings) {
        }

        bool pipeline::operator==(const pipeline& other) const noexcept {
            if (m_viewport == other.m_viewport && m_scissor == other.m_scissor && m_rasterizer == other.m_rasterizer &&
                m_multisampling == other.m_multisampling &&
                m_framebufferColorBlending == other.m_framebufferColorBlending &&
                m_globalColorBlending == other.m_globalColorBlending) {
                return true;
            }
            return false;
        }

    } // namespace base
} // namespace engine
} // namespace tst
