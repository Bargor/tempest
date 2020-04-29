// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "pipeline_settings.h"

namespace tst {
namespace engine {
    namespace base {

        bool viewport_settings::operator==(const viewport_settings& other) const noexcept {
            if (x == other.x && y == other.y && width == other.width && height == other.height &&
                minDepth == other.minDepth && maxDepth == other.maxDepth) {
                return true;
            }
            return false;
        }

        bool depth_settings::operator==(const depth_settings& other) const noexcept {
            if (depthTestEnable == other.depthTestEnable && depthWriteEnable == other.depthWriteEnable &&
                compareOperation == other.compareOperation && depthBoundsTestEnable == other.depthBoundsTestEnable &&
                minDepthBounds == other.minDepthBounds && maxDepthBounds == other.maxDepthBounds) {
                return true;
            }
            return false;
        }

        bool stencil_settings::operation_state::operator==(const stencil_settings::operation_state& other) const
            noexcept {
            if (failOperation == other.failOperation && passOperation == other.failOperation &&
                depthFailOperation == other.depthFailOperation && compareOperation == other.compareOperation &&
                compareMask == other.compareMask && writeMask == other.writeMask && reference == other.reference) {
                return true;
            }
            return false;
        }

        bool stencil_settings::operator==(const stencil_settings& other) const noexcept {
            if (enable == other.enable && frontOperation == other.frontOperation && backOperation == other.backOperation) {
                return true;
            }
            return false;
        }

        bool color_blending_settings::operator==(const color_blending_settings& other) const noexcept {
            if (enable == other.enable && colorBlendOperation == other.colorBlendOperation &&
                srcColorBlendFactor == other.srcColorBlendFactor && dstColorBlendFactor == other.dstColorBlendFactor &&
                alphaBlendOperation == other.alphaBlendOperation && srcAlphaBlendFactor == other.srcAlphaBlendFactor &&
                dstAlphaBlendFactor == other.dstAlphaBlendFactor && colorWriteMask == other.colorWriteMask) {
                return true;
            }
            return false;
        }

        bool color_blending_settings::color_component_flags::operator==(const color_component_flags& other) const
            noexcept {
            if (r == other.r && g == other.g && b == other.b && a == other.a) {
                return true;
            }
            return false;
        }

        bool global_blending_settings::operator==(const global_blending_settings& other) const noexcept {
            if (enable == other.enable && logicalOperation == other.logicalOperation &&
                blendConstants == other.blendConstants) {
                return false;
            }
            return true;
        }

        pipeline_settings::pipeline_settings(const viewport_settings& viewportSettings,
                                             const core::rectangle<std::int32_t, std::uint32_t>& scissor,
                                             const depth_settings& depthSettings,
                                             const stencil_settings& stencilSettings,
                                             const rasterizer_settings& rasterizerSettings,
                                             const multisampling_settings& multisamplingSettings,
                                             const std::vector<color_blending_settings>& blendingSettings,
                                             const global_blending_settings& globalBlendingSettings)
            : m_viewport(viewportSettings)
            , m_scissor(scissor)
            , m_depthSettings(depthSettings)
            , m_stencilSettings(stencilSettings)
            , m_rasterizer(rasterizerSettings)
            , m_multisampling(multisamplingSettings)
            , m_framebufferColorBlending(blendingSettings)
            , m_globalColorBlending(globalBlendingSettings) {
        }

        bool pipeline_settings::operator==(const pipeline_settings& other) const noexcept {
            if (m_viewport == other.m_viewport && m_scissor == other.m_scissor &&
                m_depthSettings == other.m_depthSettings && m_stencilSettings == other.m_stencilSettings &&
                m_rasterizer == other.m_rasterizer && m_multisampling == other.m_multisampling &&
                m_framebufferColorBlending == other.m_framebufferColorBlending &&
                m_globalColorBlending == other.m_globalColorBlending) {
                return true;
            }
            return false;
        }

    } // namespace base
} // namespace engine
} // namespace tst
