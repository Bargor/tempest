// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.h>

#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        class physical_device;

        class logical_device {
            friend class device_queue;
        public:
            logical_device(physical_device& physicalDevice,
                           const std::vector<const char*>& validationLayers,
                           const std::vector<const char*>& extensions,
                           bool enableValidationLayers);

            ~logical_device();
        private:
            VkDevice m_deviceHandle;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst