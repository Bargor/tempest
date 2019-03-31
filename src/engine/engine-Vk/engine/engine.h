// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "engine_init.h"

#include "queue_indices.h"

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace application {
    class data_loader;
    class main_window;
} // namespace application

namespace scene {
    class scene;
}

namespace engine {

    namespace vulkan {
        class physical_device;
        class device_queues;
        class swap_chain;
        class shader_compiler;
    } // namespace vulkan

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
        std::vector<const char*> m_requiredValidationLayers;
        std::vector<const char*> m_reqiuredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        vk::Instance m_vulkanInstance;
        vk::DebugUtilsMessengerEXT m_debugMessenger;
        vk::SurfaceKHR m_windowSurface;
        vk::PhysicalDevice m_physicalDevice;
        vulkan::queue_family_indices m_queueIndices;
        vk::Device m_logicalDevice;
        ptr<vulkan::device_queues> m_deviceQueues;
        ptr<vulkan::swap_chain> m_swapChain;
        ptr<vulkan::shader_compiler> m_shaderCompiler;
    };

} // namespace engine
} // namespace tst