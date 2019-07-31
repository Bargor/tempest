// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "gpu_info.h"

namespace tst {
namespace engine {
    namespace vulkan {
        gpu_info::gpu_info(const vk::PhysicalDevice& physicalDevice) {
            auto deviceProperties = physicalDevice.getProperties();
            //deviceProperties.
        }
    }
}
}