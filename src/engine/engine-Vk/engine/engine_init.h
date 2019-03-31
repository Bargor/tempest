// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <memory>
#include <optional>
#include <vector>
#include <vulkan/vulkan.hpp>

struct GLFWwindow;

namespace tst {
namespace engine {
    namespace vulkan {

        struct queue_family_indices;

        template<typename T>
        using ptr = std::unique_ptr<T>;

        vk::Instance init_Vulkan_instance(std::vector<const char*>& requiredValidationLayers,
                                          bool enableValidationLayers);

        vk::DebugUtilsMessengerEXT setup_debug_messenger(vk::Instance& instance, bool enableValidationLayers);

        vk::SurfaceKHR create_window_surface(vk::Instance& instance, GLFWwindow* window);

        vk::PhysicalDevice select_physical_device(vk::Instance& instance,
                                                    vk::SurfaceKHR& surface,
                                                    const std::vector<const char*>& requiredExtensions);

        vk::Device create_logical_device(const vk::PhysicalDevice& physicalDevice,
                                         const queue_family_indices& indices,
                                         const std::vector<const char*>& validationLayers,
                                         const std::vector<const char*>& extensions);

        void DestroyDebugUtilsMessengerEXT(vk::Instance& instance,
                                           vk::DebugUtilsMessengerEXT& debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);

    } // namespace vulkan
} // namespace engine
} // namespace tst