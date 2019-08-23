// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "api.h"
#include "physical_device.h"
#include "rendering_technique.h"
#include "resources/index_buffer.h"
#include "resources/shader.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <GLFW/glfw3.h>
#include <common/rectangle.h>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace application {
    template<typename Event>
    class event_processor;
    class main_window;

    struct app_event;
} // namespace application
namespace engine {
    namespace vulkan {

        class gpu_info;
        class swap_chain;

        class device {
            friend class rendering_engine;
            friend class swap_chain;

        public:
            device(application::main_window& mainWindow,
                   application::event_processor<application::app_event>& eventProcessor);
            device(const device& device) = delete;
            ~device();

            vk::CommandPool& create_command_pool();

            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<IndexType>&& indices,
                                                        const vk::CommandPool& cmdPool) const;
            vertex_buffer create_vertex_buffer(const vertex_format& format,
                                               std::vector<vertex>&& vertices,
                                               const vk::CommandPool& cmdPool) const;
            uniform_buffer create_uniform_buffer(const vk::CommandPool& cmdPool) const;
            shader crate_shader(shader::shader_type type, std::vector<char>&& source, const std::string_view name) const;
            void add_rendering_technique(const std::string& techniqueName);

            gpu_info& get_GPU_info() const noexcept;

        public:
            bool startFrame();
            bool draw(const std::vector<vk::CommandBuffer>& commandBuffers);
            bool endFrame();

        private:
            void cleanup_swap_chain_dependancies();
            void update_framebuffer();
            void recreate_swap_chain(const core::extent<std::uint32_t>& extent);

        private:
            static constexpr std::uint32_t m_maxConcurrentFrames = 2;

            std::uint32_t m_frameCounter;
            application::main_window& m_mainWindow;
            application::event_processor<application::app_event>& m_eventProcessor;
            vk::SurfaceKHR m_windowSurface;
            ptr<physical_device> m_physicalDevice;
            vk::Device m_logicalDevice;
            ptr<swap_chain> m_swapChain;
            std::vector<rendering_technique> m_techniques;
            vk::Queue m_graphicsQueueHandle;
            vk::Queue m_computeQueueHandle;
            vk::Queue m_presentationQueueHandle;
            vk::Queue m_transferQueueHandle;

            std::vector<vk::CommandPool> m_commandPools;

            std::vector<vk::Semaphore> m_imageAvailable;
            std::vector<vk::Semaphore> m_renderFinished;
            std::vector<vk::Fence> m_inFlightFences;

            bool m_framebufferResized;
        };

        template<typename IndexType>
        index_buffer<IndexType> device::create_index_buffer(std::vector<IndexType>&& indices,
                                                            const vk::CommandPool& cmdPool) const {
            return index_buffer<std::uint16_t>(m_logicalDevice,
                                               m_graphicsQueueHandle,
                                               cmdPool,
                                               m_physicalDevice->get_memory_properties(),
                                               vk::IndexType::eUint16,
                                               std::move(indices));
        }

        TST_INLINE gpu_info& device::get_GPU_info() const noexcept {
            return m_physicalDevice->get_GPU_info();
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
