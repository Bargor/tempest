// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "engine_init.h"
#include "queue_indices.h"

#include <application/event_utils.h>
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace application {
    class data_loader;
    class main_window;
    class event_processor;
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
        rendering_engine(application::main_window& mainWindow,
                         application::data_loader& dataLoader,
                         application::event_processor& eventProcessor);
        ~rendering_engine();

        void frame(size_t frameCount);
        void start();
        void stop();

    private:
        void cleanup_swap_chain_dependancies();
        void recreate_swap_chain(std::uint32_t width, std::uint32_t height);

    private:
        application::main_window& m_mainWindow;
        application::data_loader& m_dataLoader;
        application::event_processor& m_eventProcessor;
        ptr<scene::scene> m_scene;
        std::vector<const char*> m_requiredValidationLayers;
        std::vector<const char*> m_reqiuredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        static constexpr std::uint32_t m_maxConcurrentFrames = 2;

        vk::Instance m_vulkanInstance;
        vk::DebugUtilsMessengerEXT m_debugMessenger;
        vk::SurfaceKHR m_windowSurface;
        vk::PhysicalDevice m_physicalDevice;
        vulkan::queue_family_indices m_queueIndices;
        vk::Device m_logicalDevice;
        ptr<vulkan::device_queues> m_deviceQueues;
        ptr<vulkan::swap_chain> m_swapChain;
        ptr<vulkan::shader_compiler> m_shaderCompiler;
        vk::RenderPass m_renderPass;
        vk::PipelineLayout m_pipelineLayout;
        vk::Pipeline m_pipeline;
        std::vector<vk::Framebuffer> m_framebuffers;
        vk::CommandPool m_commandPool;
        std::vector<vk::CommandBuffer> m_commandBuffers;
        std::vector<vk::Semaphore> m_imageAvailable;
        std::vector<vk::Semaphore> m_renderFinished;
        std::vector<vk::Fence> m_inFlightFences;

		bool m_framebufferResized;
    };

} // namespace engine
} // namespace tst