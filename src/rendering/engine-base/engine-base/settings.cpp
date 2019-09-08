// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "settings.h"

namespace tst {
namespace engine {
    namespace base {

        bool rasterizer_settings::depth_bias_settings::operator==(const depth_bias_settings& other) const
            noexcept {
            if (enable == other.enable && constantFactor == other.constantFactor && clamp == other.clamp &&
                slopeFactor == other.slopeFactor) {
                return true;
            }
            return false;
        }

        bool rasterizer_settings::operator==(const rasterizer_settings& other) const noexcept {
            if (depthClamp == other.depthClamp && rasterizerDiscard == other.rasterizerDiscard &&
                polygonMode == other.polygonMode && cullMode == other.cullMode && frontFace == other.frontFace &&
                lineWidth == other.lineWidth && depthBias == other.depthBias) {
                return true;
            }
            return false;
        }

        bool multisampling_settings::operator==(const multisampling_settings& other) const noexcept {
            if (enable == other.enable && samples == other.samples) {
                return true;
            }
            return false;
        }

    } // namespace base
} // namespace engine
} // namespace tst
