// This file is part of Tempest project
// Author: Karol Kontny

#include "settings.h"
#include "engine-impl/settings_parser.h"

namespace tst {
namespace engine {

    settings::settings(api::settings&& impl) : api::settings(std::move(impl)) { 
    }

    settings parse_engine_settings(const application::data_loader& dataLoader) {
        return api::parse_engine_settings(dataLoader);
    }
}
} // namespace tst