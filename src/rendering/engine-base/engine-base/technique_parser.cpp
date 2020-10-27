// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "technique_parser.h"

#include <application/data_loader.h>
#include <rapidjson/document.h>

namespace tst {

namespace engine {
    namespace base {

        namespace {
            technique_settings default_settings{
                {framebuffer_settings{0,
                                      framebuffer_settings::attachment_type::color,
                                      sample_count::samples_1,
                                      framebuffer_settings::load_operation::clear,
                                      framebuffer_settings::store_operation::store,
                                      framebuffer_settings::load_operation::dont_care,
                                      framebuffer_settings::store_operation::dont_care,
                                      image_layout::undefined,
                                      image_layout::present,
                                      {0.0f, 0.0f, 0.0f, 1.0f}}},
                {subpass_settings{subpass_settings::bind_point::graphics, {0}, std::nullopt}},
                {dependency{0, 1, 1024, 1024, 0, 384}},
                false};
        }

        std::vector<framebuffer_settings> parse_framebuffers(const rapidjson::Value& framebuffersSettings) {
            assert(framebuffersSettings.IsArray());
            std::vector<framebuffer_settings> framebuffers;
            framebuffers.reserve(framebuffersSettings.GetArray().Size());

            for (const auto& framebuffer : framebuffersSettings.GetArray()) {
                const auto id = framebuffer["id"].GetUint();
                const auto type = static_cast<framebuffer_settings::attachment_type>(framebuffer["type"].GetUint());
                const auto samples = static_cast<base::sample_count>(framebuffer["samples"].GetUint());
                const auto loadOp = static_cast<framebuffer_settings::load_operation>(framebuffer["load_op"].GetUint());
                const auto storeOp =
                    static_cast<framebuffer_settings::store_operation>(framebuffer["store_op"].GetUint());
                const auto stencilLoadOp =
                    static_cast<framebuffer_settings::load_operation>(framebuffer["stencil_load_op"].GetUint());
                const auto stencolStoreOp =
                    static_cast<framebuffer_settings::store_operation>(framebuffer["stencil_store_op"].GetUint());
                const auto initialLayout = static_cast<image_layout>(framebuffer["initial_layout"].GetUint());
                const auto finalLayout = static_cast<image_layout>(framebuffer["final_layout"].GetUint());
                auto parseClearValues = [type, &framebuffer]() {
                    if (type == framebuffer_settings::attachment_type::color) {
                        return std::array<float, 4>{framebuffer["clear_value"][0].GetFloat(),
                                                    framebuffer["clear_value"][1].GetFloat(),
                                                    framebuffer["clear_value"][2].GetFloat(),
                                                    framebuffer["clear_value"][3].GetFloat()};
                    } else {
                        return std::array<float, 4>{framebuffer["clear_value"][0].GetFloat(),
                                                    framebuffer["clear_value"][1].GetFloat(),
                                                    0.0f,
                                                    0.0f};
                    }
                };

                framebuffers.emplace_back(framebuffer_settings{id,
                                                               type,
                                                               samples,
                                                               loadOp,
                                                               storeOp,
                                                               stencilLoadOp,
                                                               stencolStoreOp,
                                                               initialLayout,
                                                               finalLayout,
                                                               parseClearValues()});
            }
            return framebuffers;
        }

        std::vector<subpass_settings> parse_subpasses(const rapidjson::Value& subpassesSettings) {
            assert(subpassesSettings.IsArray());
            std::vector<subpass_settings> subpasses;
            subpasses.reserve(subpassesSettings.GetArray().Size());
            for (const auto& subpass : subpassesSettings.GetArray()) {
                const auto bindPoint = static_cast<subpass_settings::bind_point>(subpass["bind_point"].GetUint());
                std::vector<std::uint32_t> colorAttachments;
                for (const auto& id : subpass["color_attachments"].GetArray()) {
                    colorAttachments.emplace_back(id.GetUint());
                }
                const std::optional<std::uint32_t> depthAttachment = subpass["depth_attachment"].IsNull() ?
                    std::nullopt :
                    std::optional<std::uint32_t>(subpass["depth_attachment"].GetUint());
                subpasses.emplace_back(subpass_settings{bindPoint, colorAttachments, depthAttachment});
            }
            return subpasses;
        }

        std::vector<dependency> parse_dependencies(const rapidjson::Value& dependenciesSettings) {
            assert(dependenciesSettings.IsArray());
            std::vector<dependency> dependencies;
            dependencies.reserve(dependenciesSettings.GetArray().Size());
            for (const auto& dep : dependenciesSettings.GetArray()) {
                const auto srcSubpass = dep["src_subpass"].GetInt();
                const auto dstSubpass = dep["dst_subpass"].GetUint();
                const auto srcStageMask = dep["src_stage_mask"].GetUint();
                const auto dstStageMask = dep["dst_stage_mask"].GetUint();
                const auto srcAccessMask = dep["src_access_mask"].GetUint();
                const auto dstAccessMask = dep["dst_access_mask"].GetUint();
                dependencies.emplace_back(dependency{static_cast<std::uint32_t>(srcSubpass),
                                                     dstSubpass,
                                                     srcStageMask,
                                                     dstStageMask,
                                                     srcAccessMask,
                                                     dstAccessMask});
            }
            return dependencies;
        }

        technique_settings parse_technique_settings(const application::data_loader& dataLoader, std::string_view name) {
            const auto techniqueFile = dataLoader.find_file((std::filesystem::path("techniques") / name).concat(".json"));
            if (!techniqueFile) {
                return default_settings;
            }
            const auto& jsonModel = dataLoader.load_json(techniqueFile.value());

            assert(jsonModel.HasMember("framebuffers"));
            assert(jsonModel.HasMember("subpasses"));
            assert(jsonModel.HasMember("dependencies"));

            const auto framebuffers = parse_framebuffers(jsonModel["framebuffers"]);
            const auto subpasses = parse_subpasses(jsonModel["subpasses"]);
            const auto dependencies = parse_dependencies(jsonModel["dependencies"]);
            bool useDepth =
                std::any_of(framebuffers.begin(), framebuffers.end(), [](const framebuffer_settings& settings) {
                    return settings.type == framebuffer_settings::attachment_type::depth;
                });
            return technique_settings{framebuffers, subpasses, dependencies, useDepth};
        }

    } // namespace base
} // namespace engine
} // namespace tst
