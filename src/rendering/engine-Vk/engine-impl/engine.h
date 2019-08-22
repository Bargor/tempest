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
            rendering_engine(application::data_loader& dataLoader,
                             application::event_processor<application::app_event>& eventProcessor,
                             device& device);
            rendering_engine(const rendering_engine& engine) = delete;
            ~rendering_engine();
            void start();
            void stop();

            template<typename Iter>
            bool draw_frame(Iter first, Iter last);

        private:
            void recreate_swap_chain(std::uint32_t width, std::uint32_t height);
            void update_framebuffer();
            void submit_command_buffer(vk::CommandBuffer& buffer);

            template<typename Iter>
            std::vector<vk::CommandBuffer> prepare_draw(Iter first, Iter last);
            vk::CommandBuffer generate_command_buffer(const draw_info& drawInfo);

        private:
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
            m_device.startFrame();
            auto commandBuffers = prepare_draw(first, last);

            m_device.draw(commandBuffers);
            m_device.endFrame();

            // m_device.m_logicalDevice.freeCommandBuffers(m_commandPool, commandBuffers);

            return true;
        }

    } // namespace vulkan

} // namespace engine
} // namespace tst
