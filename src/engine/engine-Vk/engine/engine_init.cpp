// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine_init.h"

#include "queue_indices.h"
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

                auto availableLayers = vk::enumerateInstanceLayerProperties();

                std::sort(availableLayers.begin(),
                          availableLayers.end(),
                          [](const vk::LayerProperties& a, const vk::LayerProperties& b) {
                              return a.layerName < b.layerName;
                          });

                return tst::includes(availableLayers.begin(),
                                     availableLayers.end(),
                                     requiredLayers.begin(),
                                     requiredLayers.end(),
                                     [](const vk::LayerProperties& availLayer, const char* reqLayer) {
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

        } // namespace

        vk::Instance init_Vulkan_instance(std::vector<const char*>& requiredValidationLayers,
                                          bool enableValidationLayers) {
            setup_validation_layers(requiredValidationLayers);
            auto extensions = get_required_extensions(enableValidationLayers);

            vk::ApplicationInfo appInfo(
                "Tempest", VK_MAKE_VERSION(1, 0, 0), "Tempest", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1);

            vk::InstanceCreateInfo createInfo(vk::InstanceCreateFlags(),
                                              &appInfo,
                                              static_cast<uint32_t>(requiredValidationLayers.size()),
                                              requiredValidationLayers.data(),
                                              static_cast<uint32_t>(extensions.size()),
                                              extensions.data());
            try {
                return vk::createInstance(createInfo);
            } catch (std::runtime_error& ex) {
                ex.what();
                fmt::printf("Can't initialize Vulkan!\n");
                std::exit(EXIT_FAILURE);
            }
        }

        vk::Result CreateDebugUtilsMessengerEXT(vk::Instance instance,
                                                const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                const VkAllocationCallbacks* pAllocator,
                                                VkDebugUtilsMessengerEXT* pDebugMessenger) {
            auto func = (PFN_vkCreateDebugUtilsMessengerEXT)instance.getProcAddr("vkCreateDebugUtilsMessengerEXT");
            if (func != nullptr) {
                return static_cast<vk::Result>(func(instance, pCreateInfo, pAllocator, pDebugMessenger));
            }
            return vk::Result::eErrorExtensionNotPresent;
        }

        void DestroyDebugUtilsMessengerEXT(vk::Instance& instance,
                                           vk::DebugUtilsMessengerEXT& debugMessenger,
                                           const VkAllocationCallbacks* pAllocator) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT");
            if (func != nullptr) {
                func(instance, debugMessenger, pAllocator);
            }
        }

        vk::DebugUtilsMessengerEXT setup_debug_messenger(vk::Instance& instance, bool enableValidationLayers) {
            if (!enableValidationLayers) return vk::DebugUtilsMessengerEXT();

            vk::DebugUtilsMessengerCreateFlagsEXT flags;
            vk::DebugUtilsMessageSeverityFlagsEXT messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose;

            vk::DebugUtilsMessageTypeFlagsEXT messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

            vk::DebugUtilsMessengerCreateInfoEXT createInfo(flags, messageSeverity, messageType, debug_callback);

            VkDebugUtilsMessengerEXT debugMessenger;

            try {
                if (CreateDebugUtilsMessengerEXT(instance,
                                                 &(createInfo.operator const VkDebugUtilsMessengerCreateInfoEXT&()),
                                                 nullptr,
                                                 &debugMessenger) == vk::Result::eSuccess) {
                    return debugMessenger;
                }
                throw vulkan_exception("Failed to set up debug messenger!");

            } catch (std::runtime_error&) {
                throw vulkan_exception("Failed to set up debug messenger!");
            }
        }

        vk::SurfaceKHR create_window_surface(vk::Instance& instance, GLFWwindow* window) {
            VkSurfaceKHR surface;
            if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
                throw vulkan_exception("Failed to create window surface!");
            }
            return vk::SurfaceKHR(surface);
        }

        vk::PhysicalDevice select_physical_device(vk::Instance& instance,
                                                  vk::SurfaceKHR& surface,
                                                  const std::vector<const char*>& requiredExtensions) {
            std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();

            if (devices.size() == 0) {
                throw vulkan_exception("Failed to find GPUs with Vulkan support!");
            }

            for (const auto& device : devices) {
                try {
                    if (!check_extensions_support(device, requiredExtensions)) {
                        throw vulkan_exception("Device is not supporting required extenstions");
                    }

                    auto indices = compute_queue_indices(device, surface);

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

    } // namespace vulkan
} // namespace engine
} // namespace tst
