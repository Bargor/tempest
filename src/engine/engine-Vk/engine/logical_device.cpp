// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "logical_device.h"

#include "physical_device.h"
#include "vulkan_exception.h"

#include <set>

namespace tst {
namespace engine {
    namespace vulkan {
        logical_device::logical_device(physical_device& physicalDevice,
                                       const std::vector<const char*>& validationLayers,
                                       const std::vector<const char*>& extensions,
                                       bool enableValidationLayers) {
            std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
            auto& indices = physicalDevice.get_queue_family_indices();
            std::set<std::uint32_t> uniqueQueueFamilies = {indices.graphicsIndex.value(),
                                                           indices.presentationIndex.value(),
                                                           indices.computeIndex.value(),
                                                           indices.transferIndex.value()};

            float queuePriority = 1.0f;
            for (uint32_t queueFamily : uniqueQueueFamilies) {
                VkDeviceQueueCreateInfo queueCreateInfo = {};
                queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueCreateInfo.queueFamilyIndex = queueFamily;
                queueCreateInfo.queueCount = 1;
                queueCreateInfo.pQueuePriorities = &queuePriority;
                queueCreateInfos.push_back(queueCreateInfo);
            }

            VkPhysicalDeviceFeatures deviceFeatures = {};

            VkDeviceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
            createInfo.pQueueCreateInfos = createInfo.pQueueCreateInfos = queueCreateInfos.data();
            createInfo.pEnabledFeatures = &deviceFeatures;
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();

            if (enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
                createInfo.ppEnabledLayerNames = validationLayers.data();
            } else {
                createInfo.enabledLayerCount = 0;
            }

            if (vkCreateDevice(physicalDevice.get_handle(), &createInfo, nullptr, &m_deviceHandle) != VK_SUCCESS) {
                throw vulkan_exception("Failed to create logical device!");
            }
        }

        logical_device::~logical_device() {
            vkDestroyDevice(m_deviceHandle, nullptr);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst