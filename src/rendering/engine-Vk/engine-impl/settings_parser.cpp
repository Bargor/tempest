// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "settings_parser.h"

#include <application/data_loader.h>
#include <engine-base/settings_parser.h>

namespace tst {
namespace engine {
    namespace vulkan {

        constexpr settings default_settings{
            base::settings::get_default_settings(),
            settings::buffering::triple_buf};

        settings parse_engine_settings(const application::data_loader& dataLoader) {
            const auto settingsFile = dataLoader.find_file(std::filesystem::path("engine_settings.json"));
            if (!settingsFile) {
                return default_settings;
            } 
            const auto& jsonModel = dataLoader.load_json(settingsFile.value());
            assert(jsonModel.HasMember("rasterizer"));
            assert(jsonModel.HasMember("multisampling"));
            assert(jsonModel.HasMember("buffering"));
            const auto buffering = jsonModel["buffering"].GetUint();
            return settings(base::parse_engine_settings(jsonModel), static_cast<settings::buffering>(buffering));
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst