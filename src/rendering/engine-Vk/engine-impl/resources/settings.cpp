// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "settings.h"

namespace tst {
namespace engine {
    namespace vulkan {

        settings::settings(base::settings&& settings, buffering buffers)
            : base::settings(settings), m_buffering(buffers) {
        }

        settings::~settings() {
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
