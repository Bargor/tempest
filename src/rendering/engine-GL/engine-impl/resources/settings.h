// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/settings.h>

namespace tst {
namespace engine {
    namespace opengl {

        class settings : public base::settings {
        public:
            settings(base::settings&& settings);
            ~settings();

        private:
        };

    } // namespace opengl
} // namespace engine
} // namespace tst
