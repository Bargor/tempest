// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "pipeline.h"

namespace tst {
namespace engine {
    namespace base {

        bool pipeline::viewport_settings::operator==(const viewport_settings& other) const noexcept {
            if (x == other.x && y == other.y && width == other.width && height == other.height &&
                minDepth == other.minDepth && maxDepth == other.maxDepth) {
                return true;
            }
            return false;
        }

        bool pipeline::rasterizer_settings::depth_bias_settings::operator==(const depth_bias_settings& other) const
            noexcept {
            if (enable == other.enable && constantFactor == other.constantFactor && clamp == other.clamp &&
                slopeFactor == other.slopeFactor) {
                return true;
            }
            return false;
        }

        bool pipeline::rasterizer_settings::operator==(const rasterizer_settings& other) const noexcept {
            if (depthClamp == other.depthClamp && rasterizerDiscard == other.rasterizerDiscard &&
                polygonMode == other.polygonMode && cullMode == other.cullMode && frontFace == other.frontFace &&
                lineWidth == other.lineWidth && depthBias == other.depthBias) {
                return true;
            }
            return false;
        }

        bool pipeline::multisampling_settings::operator==(const multisampling_settings& other) const noexcept {
            if (enable == other.enable && samples == other.samples) {
                return true;
            }
            return false;
        }

        bool pipeline::color_blending_settings::operator==(const color_blending_settings& other) const noexcept {
            if (enable == other.enable && colorBlendOperation == other.colorBlendOperation &&
                srcColorBlendFactor == other.srcColorBlendFactor && dstColorBlendFactor == other.dstColorBlendFactor &&
                alphaBlendOperation == other.alphaBlendOperation && srcAlphaBlendFactor == other.srcAlphaBlendFactor &&
                dstAlphaBlendFactor == other.dstAlphaBlendFactor && colorWriteMask == other.colorWriteMask) {
                return true;
            }
            return false;
        }

        bool pipeline::color_blending_settings::color_component_flags::operator==(const color_component_flags& other) const
            noexcept {
            if (r == other.r && g == other.g && b == other.b && a == other.a) {
                return true;
            }
            return false;
        }

        bool pipeline::global_blending_settings::operator==(const global_blending_settings& other) const noexcept {
            if (enable == other.enable && logicalOperation == other.logicalOperation &&
                blendConstants == other.blendConstants) {
                return false;
            }
            return true;
        }

        bool pipeline::operator==(const pipeline& other) const noexcept {
            if (viewport == other.viewport && scissor == other.scissor && rasterizer == other.rasterizer &&
                multisampling == other.multisampling && framebufferColorBlending == other.framebufferColorBlending &&
                globalColorBlending == other.globalColorBlending) {
                return true;
            }
            return false;
        }

    } // namespace base
} // namespace engine
} // namespace tst
