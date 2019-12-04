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
                                                 const viewport_settings& viewport,
                                                 const core::rectangle<std::int32_t, std::uint32_t> scissor,
                                                 std::vector<color_blending_settings> framebufferBlending,
                                                 const global_blending_settings& globalBlending)
            : m_techniqueName(std::move(techniqueName))
            , m_viewportSettings(viewport)
            , m_scissor(scissor)
            , m_framebufferColorBlending(framebufferBlending)
            , m_globalColorBlending(globalBlending) {
        }

        rendering_technique::rendering_technique(std::string&& techniqueName, technique_settings&& techniqueSettings)
            : m_techniqueName(std::move(techniqueName))
            , m_viewportSettings(std::move(techniqueSettings.viewport))
            , m_scissor(std::move(techniqueSettings.scissor))
            , m_framebufferColorBlending(std::move(techniqueSettings.framebufferColorBlending))
            , m_globalColorBlending(std::move(techniqueSettings.globalColorBlending)) {
        }
    } // namespace base
} // namespace engine
} // namespace tst
