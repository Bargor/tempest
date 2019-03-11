// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "physical_device.h"

#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

struct GLFWwindow;

namespace tst {
namespace engine {
    namespace vulkan {

        VkInstance init_Vulkan_instance(std::vector<const char*>& requiredValidationLayers);

        VkDebugUtilsMessengerEXT setup_debug_messenger(VkInstance& instance);

        VkSurfaceKHR create_window_surface(VkInstance& instance, GLFWwindow* window);

        physical_device select_physical_device(VkInstance& instance,
                                               VkSurfaceKHR& surface,
                                               const std::vector<const char*>& requiredExtensions);

        VkDevice create_logical_device(physical_device& physicalDevice,
                                       const std::vector<const char*>& validationLayers,
                                       const std::vector<const char*>& extensions);

        VkQueue create_queue(VkDevice& device, std::uint32_t queueFamilyIndex);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);

    } // namespace vulkan
} // namespace engine
} // namespace tst