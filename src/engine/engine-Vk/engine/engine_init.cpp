// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine_init.h"

#include "vulkan_exception.h"

#include <GLFW/glfw3.h>
#include <algorithm/algorithm.h>
#include <cstdlib>
#include <fmt/printf.h>
#include <set>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {

        namespace {

            bool validation_layers_supported(std::vector<const char*>& requiredLayers) {
                std::sort(requiredLayers.begin(), requiredLayers.end());

                uint32_t layerCount;
                vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

                std::vector<VkLayerProperties> availableLayers(layerCount);
                vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

                std::sort(
                    availableLayers.begin(),
                    availableLayers.end(),
                    [](const VkLayerProperties& a, const VkLayerProperties& b) { return a.layerName < b.layerName; });

                return tst::includes(availableLayers.begin(),
                                     availableLayers.end(),
                                     requiredLayers.begin(),
                                     requiredLayers.end(),
                                     [](const VkLayerProperties& availLayer, const char* reqLayer) {
                                         return std::string_view(availLayer.layerName) == std::string_view(reqLayer);
                                     });
            }

            void setup_validation_layers(std::vector<const char*>& requiredValidationLayers) {
                if (!validation_layers_supported(requiredValidationLayers)) {
                    throw vulkan::vulkan_exception("Validation layers requested, but not available!");
                }
            }

            std::vector<const char*> get_required_extensions(bool enableValidationLayers) {
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

        } // namespace

        VkInstance init_Vulkan_instance(std::vector<const char*>& requiredValidationLayers, bool enableValidationLayers) {
            if (enableValidationLayers) {
                setup_validation_layers(requiredValidationLayers);
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

            auto extensions = get_required_extensions(enableValidationLayers);
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

        void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator) {
            auto func =
                (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance, debugMessenger, pAllocator);
            }
        }

        VkDebugUtilsMessengerEXT setup_debug_messenger(VkInstance& instance, bool enableValidationLayers) {
            if (!enableValidationLayers) return VK_NULL_HANDLE;

            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debug_callback;

            VkDebugUtilsMessengerEXT debugMessenger;

            if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
                throw vulkan_exception("Failed to set up debug messenger!");
            }

            return debugMessenger;
        }

        VkSurfaceKHR create_window_surface(VkInstance& instance, GLFWwindow* window) {
            VkSurfaceKHR surface;
            if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
                throw vulkan_exception("Failed to create window surface!");
            }
            return surface;
        }

        ptr<physical_device> select_physical_device(VkInstance& instance,
                                               VkSurfaceKHR& surface,
                                               const std::vector<const char*>& requiredExtensions) {
            uint32_t deviceCount = 0;
            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

            if (deviceCount == 0) {
                throw vulkan_exception("Failed to find GPUs with Vulkan support!");
            }

            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

            for (const auto& device : devices) {
                try {
                    physical_device vulkanDevice(device, surface, requiredExtensions);
                    if (vulkanDevice.has_required_queues()) {
                        return std::make_unique<physical_device>(vulkanDevice);
                    }
                } catch (vulkan_exception&) {
                }
            }

            throw vulkan_exception("Failed to find a suitable GPU!");
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
