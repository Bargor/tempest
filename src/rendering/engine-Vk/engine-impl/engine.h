// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "device.h"
#include "draw_info.h"
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
                             application::event_processor<application::app_event>& eventProcessor,
                             device& device);
            rendering_engine(const rendering_engine& engine) = delete;
            ~rendering_engine();

            void frame(size_t frameCount);
            void start();
            void stop();

            template<typename Iter>
            bool draw_frame(Iter first, Iter last);

        private:
            void cleanup_swap_chain_dependancies();
            void recreate_swap_chain(std::uint32_t width, std::uint32_t height);
            void update_framebuffer();
            void update_uniform_buffer(vulkan::uniform_buffer& buffer);
            void submit_command_buffer(vk::CommandBuffer& buffer);

            template<typename Iter>
            std::vector<vk::CommandBuffer> prepare_draw(Iter first, Iter last);
            vk::CommandBuffer generate_command_buffer(const draw_info& drawInfo);

        private:
            application::main_window& m_mainWindow;
            application::data_loader& m_dataLoader;
            application::event_processor<application::app_event>& m_eventProcessor;
            static constexpr std::uint32_t m_maxConcurrentFrames = 2;

            device& m_device;
            ptr<shader_compiler> m_shaderCompiler;
            vk::RenderPass m_renderPass;
            vk::DescriptorSetLayout m_descriptorSetLayout;
            vk::PipelineLayout m_pipelineLayout;
            vk::Pipeline m_pipeline;
            std::vector<vk::Framebuffer> m_framebuffers;
            vk::CommandPool m_commandPool;
            ptr<vertex_buffer> m_vertexBuffer;
            ptr<index_buffer<std::uint16_t>> m_indexBuffer;
            std::vector<uniform_buffer> m_uniformBuffers;
            vk::DescriptorPool m_descriptorPool;
            std::vector<vk::DescriptorSet> m_descriptorSets;
            std::vector<vk::CommandBuffer> m_commandBuffers;
            std::vector<vk::Semaphore> m_imageAvailable;
            std::vector<vk::Semaphore> m_renderFinished;
            std::vector<vk::Fence> m_inFlightFences;

            bool m_framebufferResized;
        };

        template<typename Iter>
        std::vector<vk::CommandBuffer> rendering_engine::prepare_draw(Iter first, Iter last) {
            std::vector<vk::CommandBuffer> buffers;

            for (; first != last; ++first) {
                auto& drawItem = *first;
                buffers.emplace_back(generate_command_buffer(drawItem));
            }

            return buffers;
        }

        template<typename Iter>
        bool rendering_engine::draw_frame(Iter first, Iter last) {
            auto commandBuffers = prepare_draw(first, last);

            m_device.startFrame();
            m_device.draw(commandBuffers);
            m_device.endFrame();

            return true;
        }

    } // namespace vulkan

} // namespace engine
} // namespace tst
