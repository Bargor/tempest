// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "physical_device.h"

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace tst {
namespace engine {
    namespace vulkan {

        template<typename T>
        using ptr = std::unique_ptr<T>;

        VkInstance init_Vulkan_instance(std::vector<const char*>& requiredValidationLayers, bool enableValidationLayers);

        VkDebugUtilsMessengerEXT setup_debug_messenger(VkInstance& instance, bool enableValidationLayers);

        VkSurfaceKHR create_window_surface(VkInstance& instance, GLFWwindow* window);

        ptr<physical_device> select_physical_device(VkInstance& instance,
                                               VkSurfaceKHR& surface,
                                               const std::vector<const char*>& requiredExtensions);

        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);

    } // namespace vulkan
} // namespace engine
} // namespace tst