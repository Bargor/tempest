// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <vulkan/vulkan.h>
#include <optional>

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

namespace tst {
namespace engine {
    namespace vulkan {

        struct queue_family_indices {
            std::optional<uint32_t> graphicsFamily;

            bool is_complete() {
                return graphicsFamily.has_value();
            }
        };
        
        VkInstance init_Vulkan_instance();
        VkDebugUtilsMessengerEXT setup_debug_messenger(VkInstance& instance);
        VkPhysicalDevice select_physical_device(VkInstance& instance);
        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);

    }
}
}