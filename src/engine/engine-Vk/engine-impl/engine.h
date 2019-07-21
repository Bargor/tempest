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
    template<typename Event>
    class event_processor;

    struct app_event;
} // namespace application

namespace engine {

    namespace vulkan {
        class device;
        class device_queues;
        class swap_chain;
        class shader_compiler;
        class vertex_buffer;
        template<typename IndexType>
        class index_buffer;
        class uniform_buffer;

        class rendering_engine {
            template<typename T>
            using ptr = std::unique_ptr<T>;

        public:
            rendering_engine(application::main_window& mainWindow,
                             application::data_loader& dataLoader,
                             application::event_processor<application::app_event>& eventProcessor);
            ~rendering_engine();

            void frame(size_t frameCount);
            void start();
            void stop();
            device& get_GPU() const noexcept;

        private:
            void cleanup_swap_chain_dependancies();
            void recreate_swap_chain(std::uint32_t width, std::uint32_t height);
            void update_framebuffer();
            void update_uniform_buffer(vulkan::uniform_buffer& buffer);
            void submitCommandBuffer(vk::CommandBuffer& buffer);

        private:
            application::main_window& m_mainWindow;
            application::data_loader& m_dataLoader;
            application::event_processor<application::app_event>& m_eventProcessor;
            std::vector<const char*> m_reqiuredDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
            static constexpr std::uint32_t m_maxConcurrentFrames = 2;

            ptr<vulkan::device> m_device;
            ptr<vulkan::swap_chain> m_swapChain;
            ptr<vulkan::shader_compiler> m_shaderCompiler;
            vk::RenderPass m_renderPass;
            vk::DescriptorSetLayout m_descriptorSetLayout;
            vk::PipelineLayout m_pipelineLayout;
            vk::Pipeline m_pipeline;
            std::vector<vk::Framebuffer> m_framebuffers;
            vk::CommandPool m_commandPool;
            ptr<vulkan::vertex_buffer> m_vertexBuffer;
            ptr<vulkan::index_buffer<std::uint16_t>> m_indexBuffer;
            std::vector<vulkan::uniform_buffer> m_uniformBuffers;
            vk::DescriptorPool m_descriptorPool;
            std::vector<vk::DescriptorSet> m_descriptorSets;
            std::vector<vk::CommandBuffer> m_commandBuffers;
            std::vector<vk::Semaphore> m_imageAvailable;
            std::vector<vk::Semaphore> m_renderFinished;
            std::vector<vk::Fence> m_inFlightFences;

            bool m_framebufferResized;

            std::vector<vk::CommandBuffer> m_buffersToRender;
        };
    } // namespace vulkan

} // namespace engine
} // namespace tst