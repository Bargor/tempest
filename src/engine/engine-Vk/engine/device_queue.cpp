// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device_queue.h"

namespace tst {
namespace engine {

    namespace vulkan {

        device_queues::device_queues(const VkDevice& logicalDevice,
                                   const physical_device::queue_family_indices& queueFamilyIndices) {
            vkGetDeviceQueue(logicalDevice, queueFamilyIndices.graphicsIndex.value(), 0, &m_graphicsQueueHandle);
            vkGetDeviceQueue(logicalDevice, queueFamilyIndices.computeIndex.value(), 0, &m_computeQueueHandle);
            vkGetDeviceQueue(logicalDevice, queueFamilyIndices.presentationIndex.value(), 0, &m_presentationQueueHandle);
            vkGetDeviceQueue(logicalDevice, queueFamilyIndices.transferIndex.value(), 0, &m_transferQueueHandle);
        }

        device_queues::~device_queues() {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst