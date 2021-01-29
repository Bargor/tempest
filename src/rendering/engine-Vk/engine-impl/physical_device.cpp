// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "physical_device.h"

#include "gpu_info.h"
#include "instance.h"
#include "vulkan_exception.h"

#include <algorithm/algorithm.h>
#include <fmt/printf.h>
#include <set>

namespace tst {
namespace engine {
    namespace vulkan {

        bool check_extensions_support(const vk::PhysicalDevice& handle,
                                      const std::vector<const char*>& requiredExtenstions) {
            const auto availableExtensions = handle.enumerateDeviceExtensionProperties();

            return tst::includes(availableExtensions.cbegin(),
                                 availableExtensions.cend(),
                                 requiredExtenstions.cbegin(),
                                 requiredExtenstions.cend(),
                                 [](const vk::ExtensionProperties availExtension, const char* reqExtension) {
                                     return std::string_view(availExtension.extensionName) ==
                                         std::string_view(reqExtension);
                                 });
        }

        bool check_features_support(const vk::PhysicalDevice& handle, const vk::PhysicalDeviceFeatures& requiredFeatures) {
            const auto supportedFeatures = handle.getFeatures();
            if (supportedFeatures.samplerAnisotropy != requiredFeatures.samplerAnisotropy) {
                return false;
            }
            return true;
        }

        std::uint32_t rate_device(gpu_info info) {
            std::uint32_t score = 0;

            switch (info.deviceType) {
            case gpu_info::device_type::discrete:
                score += 100;
                break;
            case gpu_info::device_type::integrated:
                score += 10;
                break;
            case gpu_info::device_type::cpu:
                score += 1;
                break;
            case gpu_info::device_type::other:
                break;
            }

            return score;
        }

        ptr<physical_device> physical_device::select_physical_device(vk::SurfaceKHR surface,
                                                                     const std::vector<const char*>& requiredExtensions,
                                                                     const vk::PhysicalDeviceFeatures& requiredFeatures) {
            const auto& instance = instance::get_instance();
            std::vector<vk::PhysicalDevice> devices = instance.get_instance_handle().enumeratePhysicalDevices();

            if (devices.size() == 0) {
                throw vulkan_exception("Failed to find GPUs with Vulkan support!");
            }

            std::uint32_t maxScore = 0;
            ptr<physical_device> bestDevice;

            for (const auto& device : devices) {
                try {
                    if (!check_extensions_support(device, requiredExtensions)) {
                        throw vulkan_exception("Device is not supporting required extenstions");
                    }

                    if (!check_features_support(device, requiredFeatures)) {
                        throw vulkan_exception("Device is not supporting required features");
                    }

                    auto indices = compute_queue_indices(surface, device);
                    ptr<gpu_info> info = std::make_unique<gpu_info>(device);

                    std::uint32_t score = rate_device(device);

                    if (maxScore < score) {
                        bestDevice = std::make_unique<physical_device>(device, std::move(info), indices);
                        maxScore = score;
                    }

                } catch (vulkan_exception& ex) {
                    fmt::printf("%s\n", ex.what());
                }
            }
            if (maxScore > 0) {
                return bestDevice;
            }

            throw vulkan_exception("Failed to find a suitable GPU!");
        }

        physical_device::physical_device(vk::PhysicalDevice device, ptr<gpu_info> info, queue_family_indices& indices)
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
                                                           m_queueIndices.computeIndex.value(),
                                                           m_queueIndices.transferIndex.value()};
            if (m_queueIndices.presentationIndex.has_value())
                uniqueQueueFamilies.insert(m_queueIndices.presentationIndex.value());

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
