// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "settings.h"

namespace tst {
namespace engine {
    namespace vulkan {

        settings::settings(base::settings&& settings) : base::settings(settings) {
        }

        settings::~settings() {
        }
    }
} // namespace engine
} // namespace tst
