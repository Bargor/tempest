// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "pipeline_settings.h"

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {
    namespace base {

        draw_settings parse_draw_settings(const application::data_loader& dataLoader, const std::string& name);

    }
} // namespace engine
} // namespace tst