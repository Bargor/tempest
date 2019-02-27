// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include "scene/scene.h"

#include <GLFW/glfw3.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {

    void rendering_engine::init_Vulkan() {
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

        if (glfwExtensions == nullptr) {
            fmt::printf("Can't get required Vulkan extensions!\n");
            std::exit(EXIT_FAILURE);
        }

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &m_vulkanInstance) != VK_SUCCESS) {
            fmt::printf("Can't initialize Vulkan!\n");
            std::exit(EXIT_FAILURE);
        }
    }

    rendering_engine::rendering_engine(application::data_loader& dataLoader)
        : m_dataLoader(dataLoader), m_scene(std::make_unique<scene::scene>()) {
        init_Vulkan();
    }

    rendering_engine::~rendering_engine() {
        vkDestroyInstance(m_vulkanInstance, nullptr);
    }

    void rendering_engine::frame() {}

} // namespace engine
} // namespace tst