// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "pipeline_settings.h"

#include <string_view>

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {
    namespace base {

        draw_settings parse_draw_settings(const application::data_loader& dataLoader, std::string_view name);

    }
} // namespace engine
} // namespace tst