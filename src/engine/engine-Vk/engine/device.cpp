// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

#include "instance.h"
#include "vulkan_exception.h"

#include <GLFW/glfw3.h>
#include <algorithm/algorithm.h>
#include <fmt/printf.h>
#include <set>

namespace tst {
namespace engine {
    namespace vulkan {

        vk::SurfaceKHR create_window_surface(GLFWwindow* window) {
            VkSurfaceKHR surface;
            if (glfwCreateWindowSurface(instance::get_instance().get_instance_handle(), window, nullptr, &surface) !=
                VK_SUCCESS) {
                throw vulkan_exception("Failed to create window surface!");
            }
            return vk::SurfaceKHR(surface);
        }

        bool check_extensions_support(const vk::PhysicalDevice& handle,
                                      const std::vector<const char*>& requiredExtenstions) {
            auto availableExtensions = handle.enumerateDeviceExtensionProperties();

            return tst::includes(availableExtensions.cbegin(),
                                 availableExtensions.cend(),
                                 requiredExtenstions.cbegin(),
                                 requiredExtenstions.cend(),
                                 [](const vk::ExtensionProperties availExtension, const char* reqExtension) {
                                     return std::string_view(availExtension.extensionName) ==
                                         std::string_view(reqExtension);
                                 });
        }

        vk::PhysicalDevice select_physical_device(vk::SurfaceKHR& surface,
                                                  const std::vector<const char*>& requiredExtensions) {
            auto& instance = instance::get_instance();
            std::vector<vk::PhysicalDevice> devices = instance.get_instance_handle().enumeratePhysicalDevices();

            if (devices.size() == 0) {
                throw vulkan_exception("Failed to find GPUs with Vulkan support!");
            }

            for (const auto& device : devices) {
                try {
                    if (!check_extensions_support(device, requiredExtensions)) {
                        throw vulkan_exception("Device is not supporting required extenstions");
                    }

                    compute_queue_indices(surface, device);

                    return device;

                } catch (vulkan_exception& ex) {
                    fmt::printf("%s\n", ex.what());
                }
            }

            throw vulkan_exception("Failed to find a suitable GPU!");
        }

        vk::Device create_logical_device(const vk::PhysicalDevice& physicalDevice,
                                         const queue_family_indices& indices,
                                         const std::vector<const char*>& validationLayers,
                                         const std::vector<const char*>& extensions) {
            std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

            std::set<std::uint32_t> uniqueQueueFamilies = {indices.graphicsIndex.value(),
                                                           indices.presentationIndex.value(),
                                                           indices.computeIndex.value(),
                                                           indices.transferIndex.value()};

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
                return physicalDevice.createDevice(createInfo);
            } catch (std::runtime_error&) {
                throw vulkan_exception("Failed to create logical device!");
            }
        }

        device::device(GLFWwindow* window, const std::vector<const char*>& requiredExtensions)
            : m_windowSurface(create_window_surface(window))
            , m_physicalDevice(select_physical_device(m_windowSurface, requiredExtensions))
            , m_queueIndices(compute_queue_indices(m_windowSurface, m_physicalDevice))
            , m_logicalDevice(create_logical_device(
                  m_physicalDevice, m_queueIndices, instance::get_validation_layers(), requiredExtensions))
            , m_graphicsQueueHandle(m_logicalDevice.getQueue(m_queueIndices.graphicsIndex.value(), 0))
            , m_computeQueueHandle(m_logicalDevice.getQueue(m_queueIndices.computeIndex.value(), 0))
            , m_presentationQueueHandle(m_logicalDevice.getQueue(m_queueIndices.presentationIndex.value(), 0))
            , m_transferQueueHandle(m_logicalDevice.getQueue(m_queueIndices.transferIndex.value(), 0)) {
        }

        device::~device() {
            instance::get_instance().m_instance.destroySurfaceKHR(m_windowSurface);
            m_logicalDevice.destroy();
        }

        vk::CommandPool device::create_command_pool() const {
            vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlags(), m_queueIndices.graphicsIndex.value());

            return m_logicalDevice.createCommandPool(createInfo);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
