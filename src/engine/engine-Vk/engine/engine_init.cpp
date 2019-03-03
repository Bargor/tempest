// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine_init.h"

#include "vulkan_exception.h"

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <fmt/printf.h>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        namespace {

            bool validation_layers_supported(const std::vector<const char*>& requiredLayers) {
                uint32_t layerCount;
                vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

                std::vector<VkLayerProperties> availableLayers(layerCount);
                vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

                for (std::string layerName : requiredLayers) {
                    bool layerFound = false;

                    for (const auto& layerProperties : availableLayers) {
                        if (layerName == layerProperties.layerName) {
                            layerFound = true;
                            break;
                        }
                    }

                    if (!layerFound) {
                        return false;
                    }
                }

                return true;
            }

            std::vector<const char*> get_required_extensions() {
                uint32_t glfwExtensionCount = 0;
                const char** glfwExtensions;
                glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

                if (glfwExtensions == nullptr) {
                    fmt::printf("Can't get required Vulkan extensions!\n");
                    std::exit(EXIT_FAILURE);
                }

                std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

                if (enableValidationLayers) {
                    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                }

                return extensions;
            }

            static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                                 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                                 void*) {
                fmt::printf("validation layer: %s, messageType: %d, severity: %d\n",
                            pCallbackData->pMessage,
                            messageType,
                            messageSeverity);

                return VK_FALSE;
            }

            VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                                  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                  const VkAllocationCallbacks* pAllocator,
                                                  VkDebugUtilsMessengerEXT* pDebugMessenger) {
                auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,
                                                                                      "vkCreateDebugUtilsMessengerEXT");
                if (func != nullptr) {
                    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
                } else {
                    return VK_ERROR_EXTENSION_NOT_PRESENT;
                }
            }

            queue_family_indices find_queue_families(VkPhysicalDevice device) {
                queue_family_indices indices;

                uint32_t queueFamilyCount = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

                std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
                vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

                int i = 0;
                for (const auto& queueFamily : queueFamilies) {
                    if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT & VK_QUEUE_COMPUTE_BIT) {
                        indices.graphicsFamily = i;
                    }

                    if (indices.is_complete()) {
                        break;
                    }

                    i++;
                }

                return indices;
            }

            bool is_device_suitable(VkPhysicalDevice device) {
                queue_family_indices indices = find_queue_families(device);

                return indices.is_complete();
            }

        } // namespace

        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator) {
            auto func =
                (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance, debugMessenger, pAllocator);
            }
        }

        VkInstance init_Vulkan_instance() {
            std::vector<const char*> requiredValidationLayers = {"VK_LAYER_LUNARG_standard_validation"};

            if (enableValidationLayers && !validation_layers_supported(requiredValidationLayers)) {
                throw vulkan::vulkan_exception("validation layers requested, but not available!");
            }

            VkApplicationInfo appInfo = {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Tempest";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "Tempest";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_1;

            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            if (enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size());
                createInfo.ppEnabledLayerNames = requiredValidationLayers.data();
            } else {
                createInfo.enabledLayerCount = 0;
            }

            auto extensions = get_required_extensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();

            if (enableValidationLayers) {
                createInfo.enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size());
                createInfo.ppEnabledLayerNames = requiredValidationLayers.data();
            } else {
                createInfo.enabledLayerCount = 0;
            }

            VkInstance vulkanInstance;

            if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS) {
                fmt::printf("Can't initialize Vulkan!\n");
                std::exit(EXIT_FAILURE);
            }

            return vulkanInstance;
        }

        VkDebugUtilsMessengerEXT setup_debug_messenger(VkInstance& instance) {
            VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

            if (!enableValidationLayers) return debugMessenger;

            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debug_callback;

            if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
                throw vulkan_exception("Failed to set up debug messenger!");
            }

            return debugMessenger;
        }

        VkPhysicalDevice select_physical_device(VkInstance& instance) {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

            if (deviceCount == 0) {
                throw vulkan_exception("Failed to find GPUs with Vulkan support!");
            }

            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

            for (const auto& device : devices) {
                if (is_device_suitable(device)) {
                    physicalDevice = device;
                    break;
                }
            }

            if (physicalDevice == VK_NULL_HANDLE) {
                throw vulkan_exception("Failed to find a suitable GPU!");
            }

            return physicalDevice;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
