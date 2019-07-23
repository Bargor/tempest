// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine_frontend.h"

#include "device.h"
#include "engine_init.h"

namespace tst {
namespace engine {
    namespace vulkan {
        engine_frontend::engine_frontend(device& device)
            : m_device(device), m_commandPool(m_device.create_command_pool()) {
        }
    } // namespace vulkan

} // namespace engine
} // namespace tst