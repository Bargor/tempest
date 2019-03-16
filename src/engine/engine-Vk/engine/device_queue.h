// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "physical_device.h"

#include <cstdint>
#include <vulkan/vulkan.h>

namespace tst {
namespace engine {

    namespace vulkan {

        class device_queues {
        public:
            device_queues(const VkDevice& logicalDevice, const physical_device::queue_family_indices& queueFamilyIndices);

            ~device_queues();

        public:
            VkQueue m_graphicsQueueHandle;
            VkQueue m_computeQueueHandle;
            VkQueue m_presentationQueueHandle;
            VkQueue m_transferQueueHandle;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst