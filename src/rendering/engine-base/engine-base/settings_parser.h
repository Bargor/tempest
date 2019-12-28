// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "settings.h"
#include <rapidjson/document.h>

namespace tst {

namespace engine {
    namespace base {

        settings parse_engine_settings(const rapidjson::Document& settingFile);

    }
} // namespace engine
} // namespace tst