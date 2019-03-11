// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "physical_device.h"

#include "vulkan_exception.h"

#include <algorithm/algorithm.h>
#include <vector>

namespace tst {
namespace engine {

    namespace {
        bool checkExtensionsSupport(const VkPhysicalDevice& handle, const std::vector<const char*>& requiredExtenstions) {
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(handle, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(handle, nullptr, &extensionCount, availableExtensions.data());

            return tst::includes(availableExtensions.cbegin(),
                                 availableExtensions.cend(),
                                 requiredExtenstions.cbegin(),
                                 requiredExtenstions.cend(),
                                 [](const VkExtensionProperties availExtension, const char* reqExtension) {
                                     return std::string_view(availExtension.extensionName) == std::string_view(reqExtension);
                                 });
        }
    } // namespace

    namespace vulkan {
        physical_device::physical_device(const VkPhysicalDevice& handle,
                                         const VkSurfaceKHR& windowSurface,
                                         const std::vector<const char*>& requiredExtenstions)
            : m_deviceHandle(handle) {

            if (!checkExtensionsSupport(handle, requiredExtenstions)) {
                throw vulkan_exception("Device is not supporting required extenstions");
            }

            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(m_deviceHandle, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(m_deviceHandle, &queueFamilyCount, queueFamilies.data());

            int i = 0;
            for (const auto& queueFamily : queueFamilies) {
                if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    m_indices.graphicsIndex = i;
                }

                if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                    m_indices.computeIndex = i;
                }

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(handle, i, windowSurface, &presentSupport);

                if (queueFamily.queueCount > 0 && presentSupport) {
                    m_indices.presentationIndex = i;
                }

                if (has_required_queues()) {
                    break;
                }
                i++;
            }
        }

        bool physical_device::has_required_queues() const {
            return m_indices.graphicsIndex && m_indices.computeIndex && m_indices.presentationIndex;
        }

        const physical_device::queue_family_indices& physical_device::get_queue_family_indices() const {
            return m_indices;
        }

        VkPhysicalDevice physical_device::get_handle() const {
            return m_deviceHandle;
        }
    } // namespace vulkan

} // namespace engine
} // namespace tst