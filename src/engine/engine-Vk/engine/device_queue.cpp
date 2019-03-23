// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device_queue.h"

namespace tst {
namespace engine {

    namespace vulkan {

        device_queues::device_queues(const vk::Device& logicalDevice,
                                     const queue_family_indices& queueFamilyIndices)
            : m_graphicsQueueHandle(logicalDevice.getQueue(queueFamilyIndices.graphicsIndex.value(), 0))
            , m_computeQueueHandle(logicalDevice.getQueue(queueFamilyIndices.graphicsIndex.value(), 0))
            , m_presentationQueueHandle(logicalDevice.getQueue(queueFamilyIndices.graphicsIndex.value(), 0))
            , m_transferQueueHandle(logicalDevice.getQueue(queueFamilyIndices.graphicsIndex.value(), 0)) {
        }

        device_queues::~device_queues() {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst