// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/settings.h>

namespace tst {
namespace engine {
    namespace opengl {

        class settings : public base::settings {
        public:
            enum class buffering { double_buf = 2, triple_buf = 3 };

            settings(base::settings&& settings, buffering buffers);
            ~settings();

        private:
            buffering m_buffering;
        };

    } // namespace opengl
} // namespace engine
} // namespace tst
