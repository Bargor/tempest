// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include "device.h"
#include "instance.h"
#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "shader_compiler.h"
#include "swap_chain.h"
#include "vulkan_exception.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <application/main_window.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <util/variant.h>

namespace tst {
namespace engine {

    namespace vulkan {

        rendering_engine::rendering_engine(application::main_window& mainWindow,
                                           application::data_loader& dataLoader,
                                           application::event_processor<application::app_event>& eventProcessor)
            : m_mainWindow(mainWindow)
            , m_dataLoader(dataLoader)
            , m_eventProcessor(eventProcessor)
            , m_device(std::make_unique<vulkan::device>(m_mainWindow.get_handle(), m_reqiuredDeviceExtensions))
            , m_swapChain(std::make_unique<vulkan::swap_chain>(
                  *m_device.get(), mainWindow.get_size().width, mainWindow.get_size().height))
            , m_shaderCompiler(std::make_unique<vulkan::shader_compiler>(m_dataLoader, *m_device.get()))
            , m_renderPass(vulkan::create_render_pass((*m_device.get()).m_logicalDevice, m_swapChain->get_format()))
            , m_descriptorSetLayout(vulkan::create_descriptor_set_layout((*m_device.get()).m_logicalDevice))
            , m_pipelineLayout(vulkan::create_pipeline_layout((*m_device.get()).m_logicalDevice, m_descriptorSetLayout))
            , m_pipeline(vulkan::create_graphics_pipeline((*m_device.get()).m_logicalDevice,
                                                          m_pipelineLayout,
                                                          m_renderPass,
                                                          m_swapChain->get_extent(),
                                                          *m_shaderCompiler.get()))
            , m_framebuffers(vulkan::create_framebuffers((*m_device.get()).m_logicalDevice,
                                                         m_renderPass,
                                                         m_swapChain->get_image_views(),
                                                         m_swapChain->get_extent()))
            , m_commandPool(m_device->create_command_pool())
            , m_vertexBuffer(std::make_unique<vulkan::vertex_buffer>(
                  m_device->create_vertex_buffer(vertex_format(),
                                                 std::vector<vertex>({{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                                              {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                                              {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                                              {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}}),
                                                 m_commandPool)))
            , m_indexBuffer(std::make_unique<vulkan::index_buffer<std::uint16_t>>(
                  m_device->create_index_buffer(std::vector<std::uint16_t>({{0, 1, 2, 2, 3, 0}}), m_commandPool)))
            , m_uniformBuffers(
                  vulkan::create_uniform_buffers(*m_device.get(), m_commandPool, m_swapChain->get_image_views().size()))
            , m_descriptorPool(vulkan::create_descriptor_pool((*m_device.get()).m_logicalDevice,
                                                              m_swapChain->get_image_views().size()))
            , m_descriptorSets(vulkan::create_descriptor_sets((*m_device.get()).m_logicalDevice,
                                                              m_swapChain->get_image_views().size(),
                                                              m_descriptorPool,
                                                              m_descriptorSetLayout,
                                                              m_uniformBuffers))
            , m_commandBuffers(vulkan::create_command_buffers((*m_device.get()).m_logicalDevice,
                                                              m_commandPool,
                                                              m_framebuffers,
                                                              m_renderPass,
                                                              m_pipeline,
                                                              m_swapChain->get_extent(),
                                                              *m_vertexBuffer.get(),
                                                              *m_indexBuffer.get(),
                                                              m_pipelineLayout,
                                                              m_descriptorSets))
            , m_imageAvailable({(*m_device.get()).m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo()),
                                (*m_device.get()).m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo())})
            , m_renderFinished({(*m_device.get()).m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo()),
                                (*m_device.get()).m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo())})
            , m_inFlightFences(
                  {(*m_device.get()).m_logicalDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled)),
                   (*m_device.get()).m_logicalDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled))})
            , m_framebufferResized(false) {
            auto framebufferResizeCallback = [&](const application::app_event::arguments&) {
                m_framebufferResized = true;
            };

            m_eventProcessor.subscribe(
                core::variant_index<application::app_event::arguments, application::app_event::framebuffer>(),
                this,
                std::move(framebufferResizeCallback));
        }

        rendering_engine::~rendering_engine() {
            cleanup_swap_chain_dependancies();

            (*m_device.get()).m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);

            m_vertexBuffer.reset();
            for (auto& semaphore : m_imageAvailable) {
                (*m_device.get()).m_logicalDevice.destroySemaphore(semaphore);
            }
            for (auto& semaphore : m_renderFinished) {
                (*m_device.get()).m_logicalDevice.destroySemaphore(semaphore);
            }
            for (auto& fence : m_inFlightFences) {
                (*m_device.get()).m_logicalDevice.destroyFence(fence);
            }
            m_swapChain.reset();
        }

        void rendering_engine::cleanup_swap_chain_dependancies() {
            for (auto framebuffer : m_framebuffers) {
                (*m_device.get()).m_logicalDevice.destroyFramebuffer(framebuffer);
            }

            (*m_device.get()).m_logicalDevice.freeCommandBuffers(m_commandPool, m_commandBuffers);

            (*m_device.get()).m_logicalDevice.destroyPipeline(m_pipeline);
            (*m_device.get()).m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
            (*m_device.get()).m_logicalDevice.destroyRenderPass(m_renderPass);

            (*m_device.get()).m_logicalDevice.destroyDescriptorPool(m_descriptorPool);
        }

        void rendering_engine::recreate_swap_chain(std::uint32_t width, std::uint32_t height) {
            (*m_device.get()).m_logicalDevice.waitIdle();

            cleanup_swap_chain_dependancies();

            m_swapChain.reset();

            auto newSwapChain = std::make_unique<vulkan::swap_chain>(*m_device.get(), width, height);

            m_swapChain = std::move(newSwapChain);
            m_renderPass = vulkan::create_render_pass((*m_device.get()).m_logicalDevice, m_swapChain->get_format());
            m_pipelineLayout = vulkan::create_pipeline_layout((*m_device.get()).m_logicalDevice, m_descriptorSetLayout);
            m_pipeline = vulkan::create_graphics_pipeline((*m_device.get()).m_logicalDevice,
                                                          m_pipelineLayout,
                                                          m_renderPass,
                                                          m_swapChain->get_extent(),
                                                          *m_shaderCompiler.get());
            m_framebuffers = vulkan::create_framebuffers((*m_device.get()).m_logicalDevice,
                                                         m_renderPass,
                                                         m_swapChain->get_image_views(),
                                                         m_swapChain->get_extent());
            m_uniformBuffers =
                vulkan::create_uniform_buffers(*m_device.get(), m_commandPool, m_swapChain->get_image_views().size()),
            m_descriptorPool =
                vulkan::create_descriptor_pool((*m_device.get()).m_logicalDevice, m_swapChain->get_image_views().size());
            m_descriptorSets = vulkan::create_descriptor_sets((*m_device.get()).m_logicalDevice,
                                                              m_swapChain->get_image_views().size(),
                                                              m_descriptorPool,
                                                              m_descriptorSetLayout,
                                                              m_uniformBuffers);
            m_commandBuffers = vulkan::create_command_buffers((*m_device.get()).m_logicalDevice,
                                                              m_commandPool,
                                                              m_framebuffers,
                                                              m_renderPass,
                                                              m_pipeline,
                                                              m_swapChain->get_extent(),
                                                              *m_vertexBuffer.get(),
                                                              *m_indexBuffer.get(),
                                                              m_pipelineLayout,
                                                              m_descriptorSets);
        }

        void rendering_engine::update_framebuffer() {
            m_framebufferResized = false;

            std::int32_t width, height;
            glfwGetFramebufferSize(m_mainWindow.get_handle(), &width, &height);
            m_mainWindow.set_size({width, height});
            recreate_swap_chain(width, height);
        }

        void rendering_engine::update_uniform_buffer(vulkan::uniform_buffer& buffer) {
            static auto startTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            vulkan::uniform_buffer_object ubo;
            ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.proj =
                glm::perspective(glm::radians(45.0f),
                                 m_swapChain->get_extent().width / static_cast<float>(m_swapChain->get_extent().width),
                                 0.1f,
                                 10.0f);
            ubo.proj[1][1] *= -1;

            buffer.update_buffer(ubo);
        }

        void rendering_engine::submit_command_buffer(vk::CommandBuffer& buffer) {
            m_buffersToRender.push_back(buffer);
        }

        std::vector<vk::CommandBuffer> rendering_engine::prepare_draw(std::vector<draw_info>&& infos) {
            std::vector<vk::CommandBuffer> buffers;

            for (auto& info : infos) {
                info;
            }

            return buffers;
        }

        void rendering_engine::frame(size_t frameCount) {
            std::uint32_t currentFrame = frameCount % m_maxConcurrentFrames;

            (*m_device.get())
                .m_logicalDevice.waitForFences(
                    1, &m_inFlightFences[currentFrame], true, std::numeric_limits<uint64_t>::max());

            auto acquireResult = (*m_device.get())
                                     .m_logicalDevice.acquireNextImageKHR(m_swapChain->get_native_swapchain(),
                                                                          std::numeric_limits<uint64_t>::max(),
                                                                          m_imageAvailable[currentFrame],
                                                                          vk::Fence());

            if (acquireResult.result == vk::Result::eErrorOutOfDateKHR) {
                update_framebuffer();
                return;
            } else if (acquireResult.result != vk::Result::eSuccess &&
                       acquireResult.result != vk::Result::eSuboptimalKHR) {
                throw vulkan::vulkan_exception("Failed to acquire image");
            }
            std::uint32_t imageIndex = acquireResult.value;

            vk::Semaphore waitSemaphores[] = {m_imageAvailable[currentFrame]};
            vk::Semaphore signalSemaphores[] = {m_renderFinished[currentFrame]};
            vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

            update_uniform_buffer(m_uniformBuffers[imageIndex]);

            vk::SubmitInfo submitInfo(
                1, waitSemaphores, waitStages, 1, &m_commandBuffers[imageIndex], 1, signalSemaphores);

            (*m_device.get()).m_logicalDevice.resetFences(1, &m_inFlightFences[currentFrame]);

            (*m_device.get()).m_graphicsQueueHandle.submit(1, &submitInfo, m_inFlightFences[currentFrame]);

            vk::PresentInfoKHR presentInfo(1, signalSemaphores, 1, &m_swapChain->get_native_swapchain(), &imageIndex);

            auto presentResult = (*m_device.get()).m_presentationQueueHandle.presentKHR(presentInfo);

            if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR ||
                m_framebufferResized) {
                update_framebuffer();
            } else if (presentResult != vk::Result::eSuccess) {
                throw vulkan::vulkan_exception("Failed to present image");
            }

            m_buffersToRender.clear();
        }

        void rendering_engine::start() {
        }

        void rendering_engine::stop() {
            (*m_device.get()).m_logicalDevice.waitIdle();
        }

        device& rendering_engine::get_GPU() const noexcept {
            return *m_device;
        }

        bool rendering_engine::draw_frame(std::vector<draw_info>&& infos) {

            auto commandBuffers = prepare_draw(std::move(infos));

            for(auto& commandBuffer : commandBuffers) {
                submit_command_buffer(commandBuffer);
            }
            
            return true;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst