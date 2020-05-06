// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "physical_device.h"

#include "gpu_info.h"
#include "vulkan_exception.h"

#include <set>

namespace tst {
namespace engine {
    namespace vulkan {

        physical_device::physical_device(vk::PhysicalDevice device,
                                         ptr<gpu_info> info,
                                         queue_family_indices& indices)
            : m_physicalDevice(device), m_gpuInfo(std::move(info)), m_queueIndices(indices) {
        }

        surface_support_info physical_device::get_surface_support_info(vk::SurfaceKHR windowSurface) const {
            surface_support_info info;

            info.capabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(windowSurface);
            info.formats = m_physicalDevice.getSurfaceFormatsKHR(windowSurface);
            info.presentModes = m_physicalDevice.getSurfacePresentModesKHR(windowSurface);

            return info;
        }

        vk::Device physical_device::create_logical_device(const std::vector<const char*>& validationLayers,
                                                          const std::vector<const char*>& extensions,
                                                          const vk::PhysicalDeviceFeatures& requiredFeatures) {
            std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

            std::set<std::uint32_t> uniqueQueueFamilies = {m_queueIndices.graphicsIndex.value(),
                                                           m_queueIndices.presentationIndex.value(),
                                                           m_queueIndices.computeIndex.value(),
                                                           m_queueIndices.transferIndex.value()};

            float queuePriority = 1.0f;
            for (uint32_t queueFamily : uniqueQueueFamilies) {
                vk::DeviceQueueCreateInfo queueCreateInfo(vk::DeviceQueueCreateFlags(), queueFamily, 1, &queuePriority);
                queueCreateInfos.push_back(queueCreateInfo);
            }

            vk::DeviceCreateInfo createInfo(vk::DeviceCreateFlags(),
                                            static_cast<uint32_t>(queueCreateInfos.size()),
                                            queueCreateInfos.data(),
                                            static_cast<uint32_t>(validationLayers.size()),
                                            validationLayers.data(),
                                            static_cast<uint32_t>(extensions.size()),
                                            extensions.data(),
                                            &requiredFeatures);

            try {
                return m_physicalDevice.createDevice(createInfo);
            } catch (std::runtime_error&) {
                throw vulkan_exception("Failed to create logical device!");
            }
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
