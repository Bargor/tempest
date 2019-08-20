// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include "draw_info.h"
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
                                           application::event_processor<application::app_event>& eventProcessor,
                                           device& device)
            : m_mainWindow(mainWindow)
            , m_dataLoader(dataLoader)
            , m_eventProcessor(eventProcessor)
            , m_device(device)
            , m_shaderCompiler(std::make_unique<shader_compiler>(m_dataLoader, m_device))
            , m_renderPass(vulkan::create_render_pass(m_device.m_logicalDevice, m_device.m_swapChain->get_format()))
            , m_descriptorSetLayout(vulkan::create_descriptor_set_layout(m_device.m_logicalDevice))
            , m_pipelineLayout(vulkan::create_pipeline_layout(m_device.m_logicalDevice, m_descriptorSetLayout))
            , m_pipeline(vulkan::create_graphics_pipeline(m_device.m_logicalDevice,
                                                          m_pipelineLayout,
                                                          m_renderPass,
                                                          m_device.m_swapChain->get_extent(),
                                                          *m_shaderCompiler.get()))
            , m_framebuffers(vulkan::create_framebuffers(m_device.m_logicalDevice,
                                                         m_renderPass,
                                                         m_device.m_swapChain->get_image_views(),
                                                         m_device.m_swapChain->get_extent()))
            , m_commandPool(m_device.create_command_pool())
            , m_vertexBuffer(std::make_unique<vertex_buffer>(
                  m_device.create_vertex_buffer(vertex_format(),
                                                std::vector<vertex>({{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                                     {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                                     {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                                     {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}}),
                                                m_commandPool)))
            , m_indexBuffer(std::make_unique<index_buffer<std::uint16_t>>(
                  m_device.create_index_buffer(std::vector<std::uint16_t>({{0, 1, 2, 2, 3, 0}}), m_commandPool)))
            , m_uniformBuffers(
                  create_uniform_buffers(m_device, m_commandPool, m_device.m_swapChain->get_image_views().size()))
            , m_descriptorPool(
                  create_descriptor_pool(m_device.m_logicalDevice, m_device.m_swapChain->get_image_views().size()))
            , m_descriptorSets(create_descriptor_sets(m_device.m_logicalDevice,
                                                      m_device.m_swapChain->get_image_views().size(),
                                                      m_descriptorPool,
                                                      m_descriptorSetLayout,
                                                      m_uniformBuffers))
            , m_commandBuffers(create_command_buffers(m_device.m_logicalDevice,
                                                      m_commandPool,
                                                      m_framebuffers,
                                                      m_renderPass,
                                                      m_pipeline,
                                                      m_device.m_swapChain->get_extent(),
                                                      *m_vertexBuffer.get(),
                                                      *m_indexBuffer.get(),
                                                      m_pipelineLayout,
                                                      m_descriptorSets))
            , m_imageAvailable({m_device.m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo()),
                                m_device.m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo())})
            , m_renderFinished({m_device.m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo()),
                                m_device.m_logicalDevice.createSemaphore(vk::SemaphoreCreateInfo())})
            , m_inFlightFences(
                  {m_device.m_logicalDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled)),
                   m_device.m_logicalDevice.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled))})
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

            m_device.m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);

            m_vertexBuffer.reset();
            for (auto& semaphore : m_imageAvailable) {
                m_device.m_logicalDevice.destroySemaphore(semaphore);
            }
            for (auto& semaphore : m_renderFinished) {
                m_device.m_logicalDevice.destroySemaphore(semaphore);
            }
            for (auto& fence : m_inFlightFences) {
                m_device.m_logicalDevice.destroyFence(fence);
            }
            m_device.m_swapChain.reset();
        }

        void rendering_engine::cleanup_swap_chain_dependancies() {
            for (auto framebuffer : m_framebuffers) {
                m_device.m_logicalDevice.destroyFramebuffer(framebuffer);
            }

            m_device.m_logicalDevice.freeCommandBuffers(m_commandPool, m_commandBuffers);

            m_device.m_logicalDevice.destroyPipeline(m_pipeline);
            m_device.m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
            m_device.m_logicalDevice.destroyRenderPass(m_renderPass);

            m_device.m_logicalDevice.destroyDescriptorPool(m_descriptorPool);
        }

        void rendering_engine::recreate_swap_chain(std::uint32_t width, std::uint32_t height) {
            m_device.m_logicalDevice.waitIdle();

            cleanup_swap_chain_dependancies();

            m_device.m_swapChain.reset();

            auto newSwapChain = std::make_unique<swap_chain>(
                m_device.m_logicalDevice,
                m_device.m_windowSurface,
                m_device.m_physicalDevice->get_surface_support_info(m_device.m_windowSurface),
                m_device.m_physicalDevice->get_graphics_index(),
                m_device.m_physicalDevice->get_presentation_index(),
                width,
                height);

            m_device.m_swapChain = std::move(newSwapChain);
            m_renderPass = create_render_pass(m_device.m_logicalDevice, m_device.m_swapChain->get_format());
            m_pipelineLayout = create_pipeline_layout(m_device.m_logicalDevice, m_descriptorSetLayout);
            m_pipeline = create_graphics_pipeline(m_device.m_logicalDevice,
                                                  m_pipelineLayout,
                                                  m_renderPass,
                                                  m_device.m_swapChain->get_extent(),
                                                  *m_shaderCompiler.get());
            m_framebuffers = create_framebuffers(m_device.m_logicalDevice,
                                                 m_renderPass,
                                                 m_device.m_swapChain->get_image_views(),
                                                 m_device.m_swapChain->get_extent());
            m_uniformBuffers =
                create_uniform_buffers(m_device, m_commandPool, m_device.m_swapChain->get_image_views().size());
            m_descriptorPool =
                create_descriptor_pool(m_device.m_logicalDevice, m_device.m_swapChain->get_image_views().size());
            m_descriptorSets = create_descriptor_sets(m_device.m_logicalDevice,
                                                      m_device.m_swapChain->get_image_views().size(),
                                                      m_descriptorPool,
                                                      m_descriptorSetLayout,
                                                      m_uniformBuffers);
            m_commandBuffers = create_command_buffers(m_device.m_logicalDevice,
                                                      m_commandPool,
                                                      m_framebuffers,
                                                      m_renderPass,
                                                      m_pipeline,
                                                      m_device.m_swapChain->get_extent(),
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
            ubo.proj = glm::perspective(
                glm::radians(45.0f),
                m_device.m_swapChain->get_extent().width / static_cast<float>(m_device.m_swapChain->get_extent().width),
                0.1f,
                10.0f);
            ubo.proj[1][1] *= -1;

            buffer.update_buffer(ubo);
        }

        vk::CommandBuffer rendering_engine::generate_command_buffer(const draw_info& drawInfo) {
            vk::CommandBufferAllocateInfo bufferAllocateInfo(m_commandPool, vk::CommandBufferLevel::ePrimary, 1);

            auto commandBuffers = m_device.m_logicalDevice.allocateCommandBuffers(bufferAllocateInfo);

            vk::CommandBufferBeginInfo commandBufferInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr);
            commandBuffers[0].begin(commandBufferInfo);

            vk::ClearValue clearColor = vk::ClearValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

            vk::RenderPassBeginInfo renderPassInfo(m_renderPass,
                                                   m_framebuffers[m_device.m_swapChain->get_image_index()],
                                                   vk::Rect2D({0, 0}, m_device.m_swapChain->get_extent()),
                                                   1,
                                                   &clearColor);

            commandBuffers[0].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
            commandBuffers[0].bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline);

            std::vector<vk::Buffer> vertexBuffers = {drawInfo.m_vertices->get_handle()};
            std::vector<vk::DeviceSize> offsets = {0};
            commandBuffers[0].bindVertexBuffers(0, vertexBuffers, offsets);
            commandBuffers[0].bindIndexBuffer(drawInfo.m_indices->get_handle(), 0, vk::IndexType::eUint16);
            commandBuffers[0].bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                 m_pipelineLayout,
                                                 0,
                                                 m_descriptorSets[m_device.m_swapChain->get_image_index()],
                                                 {});
            commandBuffers[0].drawIndexed(drawInfo.m_indices->get_index_count(), 1, 0, 0, 0);
            commandBuffers[0].endRenderPass();
            commandBuffers[0].end();

            return commandBuffers[0];
        }

        void rendering_engine::frame(size_t frameCount) {
            std::uint32_t currentFrame = frameCount % m_maxConcurrentFrames;

            m_device.m_logicalDevice.waitForFences(
                1, &m_inFlightFences[currentFrame], true, std::numeric_limits<uint64_t>::max());

            auto acquireResult =
                m_device.m_logicalDevice.acquireNextImageKHR(m_device.m_swapChain->get_native_swapchain(),
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

            m_device.m_logicalDevice.resetFences(1, &m_inFlightFences[currentFrame]);

            m_device.m_graphicsQueueHandle.submit(1, &submitInfo, m_inFlightFences[currentFrame]);

            vk::PresentInfoKHR presentInfo(
                1, signalSemaphores, 1, &m_device.m_swapChain->get_native_swapchain(), &imageIndex);

            auto presentResult = m_device.m_presentationQueueHandle.presentKHR(presentInfo);

            if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR ||
                m_framebufferResized) {
                update_framebuffer();
            } else if (presentResult != vk::Result::eSuccess) {
                throw vulkan::vulkan_exception("Failed to present image");
            }
        }

        void rendering_engine::start() {
        }

        void rendering_engine::stop() {
            m_device.m_logicalDevice.waitIdle();
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
