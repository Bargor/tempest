// This file is part of Tempest project
// Author: Karol Kontny

#include "settings_parser.h"

namespace tst {

namespace engine {
    namespace base {

        rasterizer_settings parse_rasterizer_settings(const rapidjson::Value& rasterizerSettings) {
            const auto clamp = rasterizerSettings["depth_clamp"].GetBool();
            const auto rasterizerDiscard = rasterizerSettings["rasterizer_discard"].GetBool();
            const auto polygonMode =
                static_cast<rasterizer_settings::polygon_mode>(rasterizerSettings["polygon_mode"].GetUint());
            const auto cullMode = static_cast<rasterizer_settings::cull_mode>(rasterizerSettings["cull_mode"].GetUint());
            const auto frontFace =
                static_cast<rasterizer_settings::front_face>(rasterizerSettings["front_face"].GetUint());
            const auto lineWidth = rasterizerSettings["line_width"].GetFloat();

            const auto& depthBiasSettings = rasterizerSettings["depth_bias"];
            const auto depthBiasEnable = depthBiasSettings["enable"].GetBool();
            const auto depthBiasConstantFactor = depthBiasSettings["constant_factor"].GetFloat();
            const auto depthBiasClamp = depthBiasSettings["clamp"].GetFloat();
            const auto depthBiasSlopeFactor = depthBiasSettings["slope_factor"].GetFloat();

            return rasterizer_settings{clamp,
                                       rasterizerDiscard,
                                       polygonMode,
                                       cullMode,
                                       frontFace,
                                       lineWidth,
                                       {depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor}};
        }

        multisampling_settings parse_multisampling_settings(const rapidjson::Value& multisamplingSettings) {
            const auto enable = multisamplingSettings["enable"].GetBool();
            const auto samples = static_cast<base::sample_count>(multisamplingSettings["samples"].GetUint());
            return multisampling_settings{enable, samples};
        }

        settings parse_engine_settings(const rapidjson::Document& settingFile) {
            return settings{parse_rasterizer_settings(settingFile["rasterizer"]),
                            parse_multisampling_settings(settingFile["multisampling"])};
        }

    } // namespace base
} // namespace engine
} // namespace tst