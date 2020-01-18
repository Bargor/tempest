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

        technique_settings parse_rendering_technique(const application::data_loader& dataLoader, const std::string& name);

    }
} // namespace engine
} // namespace tst