// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "settings_parser.h"

#include <application/data_loader.h>
#include <engine-base/settings_parser.h>

namespace tst {
namespace engine {
    namespace opengl {

        settings parse_engine_settings(const application::data_loader& dataLoader) {
            auto settingsFile = dataLoader.find_file(std::filesystem::path("engine_settings.json"));
            if (settingsFile) {
                const auto& jsonModel = dataLoader.load_json(settingsFile.value());
                assert(jsonModel.HasMember("rasterizer"));
                assert(jsonModel.HasMember("multisampling"));
                return settings(base::parse_engine_settings(jsonModel));
            } else {
                return base::settings::get_default_settings();
            }
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst