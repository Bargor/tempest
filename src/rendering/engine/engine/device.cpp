// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

namespace tst {
namespace engine {
    device::device(GLFWwindow* window, const std::vector<const char*>& requiredExtensions) : api::device(window, requiredExtensions) {
    }

    device::~device() {
    }

} // namespace engine
} // namespace tst