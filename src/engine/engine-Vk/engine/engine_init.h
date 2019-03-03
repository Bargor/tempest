// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

namespace tst {
namespace engine {
    namespace vulkan {

        struct device_queue_families {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> computeFamily;

            bool is_complete() {
                return graphicsFamily.has_value() && computeFamily.has_value();
            }
        };
        void setup_validation_layers(const std::vector<const char*>& requiredValidationLayers);
        
        VkInstance init_Vulkan_instance(const std::vector<const char*>& requiredValidationLayers);

        VkDebugUtilsMessengerEXT setup_debug_messenger(VkInstance& instance);

        VkPhysicalDevice select_physical_device(VkInstance& instance);

        device_queue_families find_queue_families(VkPhysicalDevice& device);

        VkDevice create_logical_device(VkPhysicalDevice& physicalDevice,
                                       const std::vector<const char*>& validationLayers);
        VkQueue create_graphics_queue(VkDevice& device, device_queue_families& families);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);

    }
}
}