// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device_queue.h"
#include "logical_device.h"

namespace tst {
namespace engine {

    namespace vulkan {

        device_queue::device_queue(logical_device& device, std::uint32_t queueFamilyIndex) {
            vkGetDeviceQueue(device.m_deviceHandle, queueFamilyIndex, 0, &m_queueHandle);
        }

        device_queue::~device_queue() {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst