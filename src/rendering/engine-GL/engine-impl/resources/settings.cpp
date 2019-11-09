// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "settings.h"

namespace tst {
namespace engine {
    namespace opengl {

        settings::settings(base::settings&& settings) : base::settings(settings) {
        }

        settings::~settings() {
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
