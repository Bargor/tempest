// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "queue_indices.h"

#include "vulkan_exception.h"

#include <algorithm/algorithm.h>
#include <vector>

namespace tst {
namespace engine {

    namespace vulkan {

        queue_family_indices compute_queue_indices(const vk::SurfaceKHR& windowSurface,
                                                   const vk::PhysicalDevice& physicalDevice) {
            auto queueFamilies = physicalDevice.getQueueFamilyProperties();

            queue_family_indices indices;

            int i = 0;
            for (const auto& queueFamily : queueFamilies) {
                if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                    indices.graphicsIndex = i;
                }

                if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eCompute) {
                    indices.computeIndex = i;
                }

                if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) {
                    indices.transferIndex = i;
                }

                auto presentationSupport = physicalDevice.getSurfaceSupportKHR(i, windowSurface);

                if (queueFamily.queueCount > 0 && presentationSupport) {
                    indices.presentationIndex = i;
                }
                ++i;

                if (indices.isValid()) return indices;
            }

            throw vulkan_exception("Device is not supporting required queues");
        }
    } // namespace vulkan

} // namespace engine
} // namespace tst