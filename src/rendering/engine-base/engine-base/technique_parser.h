// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "rendering_technique.h"

namespace tst {

namespace application {
    class data_loader;
}

namespace engine {
    namespace base {

        technique_settings parse_technique_settings(const application::data_loader& dataLoader, std::string_view name);
    }
} // namespace engine
} // namespace tst