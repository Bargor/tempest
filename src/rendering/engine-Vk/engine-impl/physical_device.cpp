// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "physical_device.h"

#include "gpu_info.h"
#include "vulkan_exception.h"

#include <set>

namespace tst {
namespace engine {
    namespace vulkan {

        physical_device::physical_device(vk::PhysicalDevice device, ptr<gpu_info> info, queue_family_indices& indices)
            : m_physicalDevice(device), m_gpuInfo(std::move(info)), m_queueIndices(indices) {
        }

        vk::Device physical_device::create_logical_device(const std::vector<const char*>& validationLayers,
                                                          const std::vector<const char*>& extensions) {
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
                                            nullptr);

            try {
                return m_physicalDevice.createDevice(createInfo);
            } catch (std::runtime_error&) {
                throw vulkan_exception("Failed to create logical device!");
            }
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
