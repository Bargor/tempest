// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "settings_parser.h"

#include <application/data_loader.h>

namespace tst {
namespace engine {
    namespace vulkan {

        base::rasterizer_settings parse_rasterizer_settings(const rapidjson::Value& rasterizerSettings) {
            const auto clamp = rasterizerSettings["depth_clamp"].GetBool();
            const auto rasterizerDiscard = rasterizerSettings["rasterizer_discard"].GetBool();
            const auto polygonMode =
                static_cast<base::rasterizer_settings::polygon_mode>(rasterizerSettings["polygon_mode"].GetUint());
            const auto cullMode =
                static_cast<base::rasterizer_settings::cull_mode>(rasterizerSettings["cull_mode"].GetUint());
            const auto frontFace =
                static_cast<base::rasterizer_settings::front_face>(rasterizerSettings["front_face"].GetUint());
            const auto lineWidth = rasterizerSettings["line_width"].GetFloat();

            const auto& depthBiasSettings = rasterizerSettings["depth_bias"];
            const auto depthBiasEnable = depthBiasSettings["enable"].GetBool();
            const auto depthBiasConstantFactor = depthBiasSettings["constant_factor"].GetFloat();
            const auto depthBiasClamp = depthBiasSettings["clamp"].GetFloat();
            const auto depthBiasSlopeFactor = depthBiasSettings["slope_factor"].GetFloat();

            return base::rasterizer_settings{
                clamp,
                rasterizerDiscard,
                polygonMode,
                cullMode,
                frontFace,
                lineWidth,
                {depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor}};
        }

        base::multisampling_settings parse_multisampling_settings(const rapidjson::Value& multisamplingSettings) {
            const auto enable = multisamplingSettings["enable"].GetBool();
            const auto samples =
                static_cast<base::multisampling_settings::sample_count>(multisamplingSettings["samples"].GetUint());
            return base::multisampling_settings{enable, samples};
        }

        constexpr settings default_settings{
            base::settings{base::rasterizer_settings{false,
                                                     false,
                                                     base::rasterizer_settings::polygon_mode::fill,
                                                     base::rasterizer_settings::cull_mode::back,
                                                     base::rasterizer_settings::front_face::counter_clockwise,
                                                     1.0f,
                                                     {false, 0.0f, 0.0f, 0.0f}},
                           base::multisampling_settings{false, base::multisampling_settings::sample_count::samples_1}},
            settings::buffering::triple_buf};

        settings parse_engine_settings(const application::data_loader& dataLoader) {
            auto settingsFile = dataLoader.find_file(std::filesystem::path("engine_settings.json"));
            if (settingsFile) {
                const auto& jsonModel = dataLoader.load_json(settingsFile.value());
                assert(jsonModel.HasMember("rasterizer"));
                assert(jsonModel.HasMember("multisampling"));
                assert(jsonModel.HasMember("buffering"));
                const auto buffering = jsonModel["buffering"].GetUint();
                return settings({parse_rasterizer_settings(jsonModel["rasterizer"]),
                                 parse_multisampling_settings(jsonModel["multisampling"])},
                                static_cast<settings::buffering>(buffering));
            } else {
                return default_settings;
            }
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst