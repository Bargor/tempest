// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "rendering_technique.h"

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

        bool stencil_settings::operation_state::operator==(const stencil_settings::operation_state& other) const noexcept {
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

        rendering_technique::rendering_technique(std::string&& techniqueName,
                                                 viewport_callback&& viewportCallback,
                                                 scissor_callback&& scissorCallback,
                                                 const depth_settings& depthSettings,
                                                 const stencil_settings& stencilSettings,
                                                 std::vector<color_blending_settings>&& framebufferBlending,
                                                 const global_blending_settings& globalBlending,
                                                 core::extent<std::uint32_t> windowSize)
            : m_techniqueName(std::move(techniqueName))
            , m_viewportSettingsCallback(std::move(viewportCallback))
            , m_scissorCallback(std::move(scissorCallback))
            , m_viewportSettings(viewportCallback(windowSize))
            , m_scissor(m_scissorCallback(windowSize))
            , m_depthSettings(depthSettings)
            , m_stencilSettings(stencilSettings)
            , m_framebufferColorBlending(std::move(framebufferBlending))
            , m_globalColorBlending(globalBlending) {
        }

        rendering_technique::rendering_technique(std::string&& techniqueName,
                                                 technique_settings&& techniqueSettings,
                                                 core::extent<std::uint32_t> windowSize)
            : m_techniqueName(std::move(techniqueName))
            , m_viewportSettingsCallback(std::move(techniqueSettings.viewportCallback))
            , m_scissorCallback(std::move(techniqueSettings.scissorCallback))
            , m_viewportSettings(m_viewportSettingsCallback(windowSize))
            , m_scissor(m_scissorCallback(windowSize))
            , m_depthSettings(techniqueSettings.depthSettings)
            , m_stencilSettings(techniqueSettings.stencilSettings)
            , m_framebufferColorBlending(std::move(techniqueSettings.framebufferColorBlending))
            , m_globalColorBlending(std::move(techniqueSettings.globalColorBlending)) {
        }
    } // namespace base
} // namespace engine
} // namespace tst
