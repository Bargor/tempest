// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "init.h"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace tst {
namespace engine {
    namespace backend {

        void init_backend() {
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

            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;

            createInfo.enabledLayerCount = 0;

            if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
                throw std::runtime_error("failed to create instance!");
            }
        }

        void destroy_backend() {

        }
    } // namespace backend
} // namespace engine
} // namespace tst