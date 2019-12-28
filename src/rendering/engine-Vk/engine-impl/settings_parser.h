// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "resources/settings.h"

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {
    namespace vulkan {

        settings parse_engine_settings(const application::data_loader& dataLoader);

    }
} // namespace engine
} // namespace tst