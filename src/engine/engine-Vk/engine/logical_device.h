// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

            class physical_device;

            VkDevice create_logical_device(const physical_device& physicalDevice,
                           const std::vector<const char*>& validationLayers,
                           const std::vector<const char*>& extensions,
                           bool enableValidationLayers);

    } // namespace vulkan
} // namespace engine
} // namespace tst