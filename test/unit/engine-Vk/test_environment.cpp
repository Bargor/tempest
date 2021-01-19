// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "test_environment.h"

vk::PhysicalDeviceFeatures get_required_features() {
    vk::PhysicalDeviceFeatures requiredFeatures;
    requiredFeatures.samplerAnisotropy = true;

    return requiredFeatures;
}