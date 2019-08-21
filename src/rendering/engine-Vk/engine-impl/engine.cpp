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

        rendering_engine::rendering_engine(application::data_loader& dataLoader,
                                           application::event_processor<application::app_event>& eventProcessor,
                                           device& device)
            : m_dataLoader(dataLoader)
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
            , m_commandPool(m_device.create_command_pool()) {
        }

        rendering_engine::~rendering_engine() {
            cleanup_swap_chain_dependancies();

            m_device.m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);

            m_device.m_swapChain.reset();
        }

        void rendering_engine::cleanup_swap_chain_dependancies() {
            for (auto framebuffer : m_framebuffers) {
                m_device.m_logicalDevice.destroyFramebuffer(framebuffer);
            }

            m_device.m_logicalDevice.destroyPipeline(m_pipeline);
            m_device.m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
            m_device.m_logicalDevice.destroyRenderPass(m_renderPass);
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
            commandBuffers[0].drawIndexed(drawInfo.m_indices->get_index_count(), 1, 0, 0, 0);
            commandBuffers[0].endRenderPass();
            commandBuffers[0].end();

            return commandBuffers[0];
        }

        void rendering_engine::start() {
        }

        void rendering_engine::stop() {
            m_device.m_logicalDevice.waitIdle();
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
