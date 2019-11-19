// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/settings.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class settings : public base::settings {
        public:
            enum class buffering { double_buf = 2, triple_buf = 3 };

        public:
            settings(base::settings&& settings, buffering buffers);
            ~settings();

        public:
            buffering m_buffering;

        private:
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
