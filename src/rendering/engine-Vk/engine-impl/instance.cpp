// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "instance.h"

#include "vulkan_exception.h"

#include <GLFW/glfw3.h>
#include <algorithm/algorithm.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {
    namespace vulkan {

#ifdef NDEBUG
        constexpr bool enableValidationLayers = false;
#else
        constexpr bool enableValidationLayers = true;
#endif

        static VKAPI_ATTR vk::Bool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                               VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                               const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                               void*) {
            fmt::printf("validation layer: %s, messageType: %d, severity: %d\n",
                        pCallbackData->pMessage,
                        messageType,
                        messageSeverity);

            return false;
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

        vk::DebugUtilsMessengerEXT setup_debug_messenger(vk::Instance& instance) {
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

        bool validation_layers_supported(std::vector<const char*>& requiredLayers) {
            std::sort(requiredLayers.begin(), requiredLayers.end());

            auto availableLayers = vk::enumerateInstanceLayerProperties();

            std::sort(
                availableLayers.begin(),
                availableLayers.end(),
                [](const vk::LayerProperties& a, const vk::LayerProperties& b) { return a.layerName < b.layerName; });

            return tst::includes(availableLayers.begin(),
                                 availableLayers.end(),
                                 requiredLayers.begin(),
                                 requiredLayers.end(),
                                 [](const vk::LayerProperties& availLayer, const char* reqLayer) {
                                     return std::string_view(availLayer.layerName) == std::string_view(reqLayer);
                                 });
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

        vk::Instance create_instance(std::vector<const char*>& requiredValidationLayers) {
            if (!validation_layers_supported(requiredValidationLayers)) {
                throw vulkan::vulkan_exception("Validation layers requested, but not available!");
            }

            auto extensions = get_required_extensions();

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
                fmt::printf("%s\n", ex.what());
                fmt::printf("Can't initialize Vulkan!\n");
                std::exit(EXIT_FAILURE);
            }
        }

        const instance& instance::get_instance() noexcept {
            static instance instance(get_validation_layers());
            return instance;
        }

		const vk::Instance& instance::get_instance_handle() const noexcept {
            return m_instance;
        }

        std::vector<const char*> instance::get_validation_layers() noexcept {
            if (enableValidationLayers) {
                return {"VK_LAYER_KHRONOS_validation"};
            } else
                return {};
        }

        instance::instance(std::vector<const char*>&& requiredValidationLayers)
            : m_instance(create_instance(requiredValidationLayers))
			, m_debugMessenger(setup_debug_messenger(m_instance)) {
        }

        instance::~instance() {
            if (enableValidationLayers) {
                DestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);
            }
            m_instance.destroy();
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
