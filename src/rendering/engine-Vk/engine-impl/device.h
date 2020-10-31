// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "api.h"
#include "engine_frontend.h"
#include "physical_device.h"
#include "resource_cache.h"
#include "resource_factory.h"
#include "resources/pipeline.h"
#include "resources/settings.h"
#include "resources/uniform_buffer.h"

#include <GLFW/glfw3.h>
#include <common/rectangle.h>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <algorithm/algorithm.h>

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
        class swap_chain;

        class device {
            friend class engine_frontend;
            friend class resource_factory;

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
                   const application::data_loader& dataLoader,
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

            bool start_frame();

        public: // public Vulkan interface
            vk::CommandPool create_command_pool();

            bool draw(const std::vector<vk::CommandBuffer>& commandBuffers);
            bool end_frame();

            std::uint32_t get_resource_index() const noexcept;

        private:
            void init_gui(const application::data_loader& dataLoader);
            void update_framebuffer();
            void recreate_swap_chain(const core::extent<std::uint32_t>& extent);
            void update_engine_buffers(const core::extent<std::uint32_t>& extent);

            template<typename Iter>
            std::vector<draw_info> sort_draw_infos(Iter first, Iter last) const;

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

            uniform_buffer m_globalStaticUniforms;
            uniform_buffer m_globalDynamicUniforms;
            uniform_buffer m_viewStaticUniforms;
            uniform_buffer m_viewDynamicUniforms;
        };

        template<typename Iter>
        bool device::draw_frame(Iter first, Iter last) {
            if (std::distance(first, last) == 0) {
                return false;
            }

            const auto drawInfos = sort_draw_infos(first, last);

            m_viewStaticUniforms.update_buffer(drawInfos.begin()->viewData.get_uniforms());

            auto commandBuffers = m_engineFrontend->prepare_draw(drawInfos);

            const auto guiCommandBuffers = m_engineFrontend->prepare_gui_draw(*m_resourceCache->find_technique("gui"));

            commandBuffers.insert(commandBuffers.end(), guiCommandBuffers.begin(), guiCommandBuffers.end());

            draw(commandBuffers);
            end_frame();

            return true;
        }

        template<typename Iter>
        std::vector<draw_info> device::sort_draw_infos(Iter first, Iter last) const {
            const auto compute_mask = [](const draw_info& first, draw_info& second) {
                base::descritptor_bind_flags mask;
                if (first.pipelineHash == second.pipelineHash) {
                    mask |= base::bind_flag_bits::ePipeline;
                }
                if (first.viewData == second.viewData) {
                    mask |= base::bind_flag_bits::eView_static;
                    mask |= base::bind_flag_bits::eView_dynamic;
                }
                if (&first.meshMaterial == &second.meshMaterial) {
                    mask |= base::bind_flag_bits::eMaterial_static;
                    mask |= base::bind_flag_bits::eMaterial_dynamic;
                }
                if (&first.descriptorSets == &second.descriptorSets) {
                    mask |= base::bind_flag_bits::eObject_static;
                    mask |= base::bind_flag_bits::eObject_dynamic;
                }
                return mask;
            };

            std::for_each(first, last, [&](draw_info& drawInfo) {
                drawInfo.pipelineState = m_resourceCache->find_pipeline(drawInfo.pipelineHash);
            });
            tst::for_each_adjacent(first + 1, last, [&](const draw_info& first, draw_info& second) {
                second.descriptorBindFlags = compute_mask(first, second);
            });
            return std::vector<draw_info>(first, last);
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
