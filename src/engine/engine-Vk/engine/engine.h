// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "engine_init.h"

#include <vulkan/vulkan.h>

#include <memory>
#include <vector>

namespace tst {
namespace application {
    class data_loader;
    class main_window;
}

namespace scene {
    class scene;
}

namespace engine {

    namespace vulkan {
        class physical_device;
        class logical_device;
        class device_queue;
    }

    class rendering_engine {
        template<typename T>
        using ptr = std::unique_ptr<T>;

    public:
        rendering_engine(application::data_loader& dataLoader, application::main_window& mainWindow);
        ~rendering_engine();

        void frame();
        void start();
        void stop();

    private:

        application::data_loader& m_dataLoader;
        ptr<scene::scene> m_scene;
        std::vector<const char*> m_requiredValidationLayers = {"VK_LAYER_LUNARG_standard_validation"};
        std::vector<const char*> m_reqiuredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        VkInstance m_vulkanInstance;
        VkDebugUtilsMessengerEXT m_debugMessenger;
        VkSurfaceKHR m_windowSurface;
        ptr<vulkan::physical_device> m_physicalDevice;
        ptr<vulkan::logical_device> m_logicalDevice;
        ptr<vulkan::device_queue> m_graphicsQueue;
        ptr<vulkan::device_queue> m_presentationQueue;
        ptr<vulkan::device_queue> m_computeQueue;
        ptr<vulkan::device_queue> m_transferQueue;
    };

} // namespace engine
} // namespace tst