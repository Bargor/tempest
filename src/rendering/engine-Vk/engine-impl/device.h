// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "api.h"
#include "engine_frontend.h"
#include "physical_device.h"
#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/rendering_technique.h"
#include "resources/settings.h"
#include "resources/shader.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resource_factory.h"

#include <GLFW/glfw3.h>
#include <common/rectangle.h>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace application {
    class data_loader;
    template<typename Event>
    class event_processor;
    class main_window;

    struct app_event;
} // namespace application
namespace engine {
    namespace vulkan {

        class gpu_info;
        class resource_cache;
        class swap_chain;

        class device {
            friend class engine_frontend;

        public:
            struct frame_resources {
                frame_resources(vk::Device device);

                vk::Semaphore imageAvailable;
                vk::Semaphore renderFinished;
                vk::Fence inFlightFences;
            };

        public:
            device(application::main_window& mainWindow,
                   application::event_processor<application::app_event>& eventProcessor,
                   settings&& engineSettings);
            device(const device& device) = delete;
            ~device();

        public: // public engine interface

            resource_factory create_resource_factory(const application::data_loader& dataLoader) const;

            gpu_info& get_GPU_info() const noexcept;

            template<typename Iter>
            bool draw_frame(Iter first, Iter last);

            void start();
            void stop();

        public: // public Vulkan interface
            vk::CommandPool create_command_pool();

            bool startFrame();
            bool draw(const std::vector<vk::CommandBuffer>& commandBuffers);
            bool endFrame();

            std::uint32_t get_resource_index() const noexcept;

        private:
            void update_framebuffer();
            void recreate_swap_chain(const core::extent<std::uint32_t>& extent);

        private:
            struct framebuffer_resize {
                bool shouldResize;
                core::extent<std::uint32_t> size;
            };

            std::uint32_t m_frameCounter;
            application::main_window& m_mainWindow;
            application::event_processor<application::app_event>& m_eventProcessor;
            settings m_engineSettings;
            vk::SurfaceKHR m_windowSurface;
            ptr<physical_device> m_physicalDevice;
            vk::Device m_logicalDevice;
            ptr<swap_chain> m_swapChain;
            ptr<resource_cache> m_resourceCache;
            vk::Queue m_graphicsQueueHandle;
            vk::Queue m_computeQueueHandle;
            vk::Queue m_presentationQueueHandle;
            vk::Queue m_transferQueueHandle;

            std::vector<vk::CommandPool> m_commandPools;
            std::array<frame_resources, settings::m_inFlightFrames> m_frameResources;

            ptr<engine_frontend> m_engineFrontend;

            framebuffer_resize m_framebufferResizeInfo;
            std::uint32_t m_resourceIndex;
        };

        template<typename Iter>
        bool device::draw_frame(Iter first, Iter last) {
            if (std::distance(first, last) == 0) {
                return false;
            }

            startFrame();

            auto commandBuffers = m_engineFrontend->prepare_draw(first, last);

            draw(commandBuffers);
            endFrame();

            return true;
        }

        TST_INLINE gpu_info& device::get_GPU_info() const noexcept {
            return m_physicalDevice->get_GPU_info();
        }

        TST_INLINE std::uint32_t device::get_resource_index() const noexcept {
            return m_resourceIndex;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
