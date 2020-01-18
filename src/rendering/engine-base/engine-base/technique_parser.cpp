// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "technique_parser.h"

#include <application/data_loader.h>
#include <rapidjson/document.h>

namespace tst {

namespace engine {
    namespace base {

        technique_settings default_settings{
            viewport_settings{0, 0, 840, 525, 0.0f, 1.0f},
            core::rectangle<std::int32_t, std::uint32_t>{{0, 0}, {840, 525}},
            {color_blending_settings{false,
                                     color_blending_settings::blend_operation::add,
                                     color_blending_settings::blend_factor::one,
                                     color_blending_settings::blend_factor::zero,
                                     color_blending_settings::blend_operation::add,
                                     color_blending_settings::blend_factor::one,
                                     color_blending_settings::blend_factor::zero,
                                     {true, true, true, true}}},
            global_blending_settings{
                false, engine::base::global_blending_settings::logic_operation::copy, {0.0f, 0.0f, 0.0f, 0.0f}}};

        viewport_settings parse_viewport_settings(const rapidjson::Value& viewportSettings) {
            const auto x = viewportSettings["x"].GetInt();
            const auto y = viewportSettings["y"].GetInt();
            const auto width = viewportSettings["width"].GetInt();
            const auto height = viewportSettings["height"].GetInt();
            const auto minDepth = viewportSettings["min_depth"].GetFloat();
            const auto maxDepth = viewportSettings["max_depth"].GetFloat();
            return viewport_settings{x, y, width, height, minDepth, maxDepth};
        }

        core::rectangle<std::int32_t, std::uint32_t> parse_scissor_settings(const rapidjson::Value& scissorSettings) {
            const auto offset = scissorSettings["offset"].GetArray();
            const auto dimensions = scissorSettings["dimensions"].GetArray();
            return core::rectangle<std::int32_t, std::uint32_t>{{offset[0].GetInt(), offset[1].GetInt()},
                                                                {dimensions[0].GetUint(), dimensions[1].GetUint()}};
        }

        std::vector<color_blending_settings>
        parse_framebuffer_blending_settings(const rapidjson::Value& framebufferBlendingSettings) {
            assert(framebufferBlendingSettings.IsArray());

            std::vector<color_blending_settings> blendingSettings(framebufferBlendingSettings.Size());

            for (auto it = 0u; it < framebufferBlendingSettings.Size(); ++it) {
                blendingSettings[it].enable = framebufferBlendingSettings[it]["enable"].GetBool();
                blendingSettings[it].colorBlendOperation = static_cast<color_blending_settings::blend_operation>(
                    framebufferBlendingSettings[it]["color_blend_op"].GetUint());
                blendingSettings[it].srcColorBlendFactor = static_cast<color_blending_settings::blend_factor>(
                    framebufferBlendingSettings[it]["src_color_blend_factor"].GetUint());
                blendingSettings[it].dstColorBlendFactor = static_cast<color_blending_settings::blend_factor>(
                    framebufferBlendingSettings[it]["dst_color_blend_factor"].GetUint());
                blendingSettings[it].alphaBlendOperation = static_cast<color_blending_settings::blend_operation>(
                    framebufferBlendingSettings[it]["alpha_blend_op"].GetUint());
                blendingSettings[it].srcAlphaBlendFactor = static_cast<color_blending_settings::blend_factor>(
                    framebufferBlendingSettings[it]["src_alpha_blend_factor"].GetUint());
                blendingSettings[it].dstAlphaBlendFactor = static_cast<color_blending_settings::blend_factor>(
                    framebufferBlendingSettings[it]["dst_alpha_blend_factor"].GetUint());
                blendingSettings[it].colorWriteMask = {framebufferBlendingSettings[it]["color_write_mask"][0].GetBool(),
                                                       framebufferBlendingSettings[it]["color_write_mask"][1].GetBool(),
                                                       framebufferBlendingSettings[it]["color_write_mask"][2].GetBool(),
                                                       framebufferBlendingSettings[it]["color_write_mask"][3].GetBool()};
            }

            return blendingSettings;
        }

        global_blending_settings parse_global_blending_settings(const rapidjson::Value& globalBlendingSettings) {
            const auto enable = globalBlendingSettings["enable"].GetBool();
            const auto logicOperation = static_cast<global_blending_settings::logic_operation>(
                globalBlendingSettings["logic_operation"].GetUint());
            const auto blendConstants = std::array<float, 4>{globalBlendingSettings["blend_constants"][0].GetFloat(),
                                                             globalBlendingSettings["blend_constants"][1].GetFloat(),
                                                             globalBlendingSettings["blend_constants"][2].GetFloat(),
                                                             globalBlendingSettings["blend_constants"][3].GetFloat()};
            return global_blending_settings{enable, logicOperation, blendConstants};
        }

        technique_settings parse_rendering_technique(const application::data_loader& dataLoader, const std::string& name) {
            const auto techniqueFile = dataLoader.find_file(std::filesystem::path("techniques") / (name + ".json"));
            if (!techniqueFile) {
                return default_settings;
            }
            const auto& jsonModel = dataLoader.load_json(techniqueFile.value());

            assert(jsonModel.HasMember("viewport"));
            assert(jsonModel.HasMember("scissor"));
            assert(jsonModel.HasMember("framebuffer_blending"));
            assert(jsonModel.HasMember("global_blending"));

            const auto viewport = parse_viewport_settings(jsonModel["viewport"]);
            const auto scissor = parse_scissor_settings(jsonModel["scissor"]);
            const auto framebuffer_blending = parse_framebuffer_blending_settings(jsonModel["framebuffer_blending"]);
            const auto global_blending = parse_global_blending_settings(jsonModel["global_blending"]);
            return technique_settings{viewport, scissor, framebuffer_blending, global_blending};
        }

    } // namespace base
} // namespace engine
} // namespace tst
