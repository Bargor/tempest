// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine_frontend.h"

#include "device.h"
#include "draw_info.h"
#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/rendering_technique.h"
#include "resources/texture.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

namespace tst {
namespace engine {

    namespace vulkan {

        engine_frontend::engine_frontend(device& device)
            : m_device(device)
            , m_commandPools(
                  {m_device.create_command_pool(), m_device.create_command_pool(), m_device.create_command_pool()}) {
        }

        engine_frontend::~engine_frontend() {
            for (std::uint32_t i = 0; i < static_cast<std::uint32_t>(m_commandPools.size()); i++) {
                m_device.m_logicalDevice.freeCommandBuffers(
                    m_commandPools[i], static_cast<std::uint32_t>(m_bufferCache[i].size()), m_bufferCache[i].data());
            }
        }

        std::vector<vk::CommandBuffer> engine_frontend::prepare_draw(const std::vector<draw_info>& drawInfos) {
            std::vector<vk::CommandBuffer> buffers;

            auto rangeBegin = drawInfos.begin();
            while (rangeBegin != drawInfos.end()) {
                auto rangeEnd = rangeBegin;
                while (rangeEnd != drawInfos.end() &&
                       rangeEnd->pipelineState->get_technique() == rangeBegin->pipelineState->get_technique()) {
                    rangeEnd++;
                }
                buffers.emplace_back(generate_command_buffer(rangeBegin, rangeEnd));
                rangeBegin = rangeEnd;
            }

            return buffers;
        }

        vk::CommandBuffer engine_frontend::generate_command_buffer(std::vector<draw_info>::const_iterator begin,
                                                                   std::vector<draw_info>::const_iterator end) {
            const auto idx = m_device.get_resource_index();
            vk::CommandBufferAllocateInfo bufferAllocateInfo(m_commandPools[idx], vk::CommandBufferLevel::ePrimary, 1);

            m_device.m_logicalDevice.resetCommandPool(m_commandPools[idx], vk::CommandPoolResetFlags());

            if (m_bufferCache[idx].size() == 0) {
                m_bufferCache[idx].emplace_back(m_device.m_logicalDevice.allocateCommandBuffers(bufferAllocateInfo)[0]);
            }

            const auto commandBuffer = m_bufferCache[m_device.get_resource_index()][0];

            vk::CommandBufferBeginInfo commandBufferInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr);
            commandBuffer.begin(commandBufferInfo);
            begin->pipelineState->get_technique().generate_render_pass_info(commandBuffer, vk::SubpassContents::eInline);

            for (; begin != end; begin++) {
                const auto drawInfo = *begin;
                drawInfo.pipelineState->bind_command_buffer(commandBuffer, vk::PipelineBindPoint::eGraphics);
                bind_descriptor_sets(commandBuffer, drawInfo);
                if (drawInfo.indices) {
                    drawIndexed(commandBuffer, drawInfo);
                } else {
                    draw(commandBuffer, drawInfo);
                }
            }
            commandBuffer.endRenderPass();
            commandBuffer.end();

            return commandBuffer;
        }

        void engine_frontend::bind_descriptor_sets(vk::CommandBuffer commandBuffer, const draw_info& drawInfo) const {
            const auto viewStaticSet = m_device.m_viewStaticUniforms.get_descriptor_set();
            const auto globalStaticSet = m_device.m_globalStaticUniforms.get_descriptor_set();

            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics, drawInfo.pipelineState->get_layout(), 0, 1, &globalStaticSet, 0, nullptr);
            commandBuffer.bindDescriptorSets(
                vk::PipelineBindPoint::eGraphics, drawInfo.pipelineState->get_layout(), 2, 1, &viewStaticSet, 0, nullptr);
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                             drawInfo.pipelineState->get_layout(),
                                             4,
                                             drawInfo.meshMaterial.get_static_descriptor_set(),
                                             {});
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                             drawInfo.pipelineState->get_layout(),
                                             6,
                                             1,
                                             &drawInfo.descriptorSets[0],
                                             0,
                                             nullptr);
        }

        void engine_frontend::draw(vk::CommandBuffer commandBuffer, const draw_info& drawInfo) const {
            std::vector<vk::Buffer> vertexBuffers = {drawInfo.vertices.get_handle()};
            std::vector<vk::DeviceSize> offsets = {0};
            commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);
            commandBuffer.draw(drawInfo.vertices.get_vertex_count(), 1, 0, 0);
        }

        void engine_frontend::drawIndexed(vk::CommandBuffer commandBuffer, const draw_info& drawInfo) const {
            std::vector<vk::Buffer> vertexBuffers = {drawInfo.vertices.get_handle()};
            std::vector<vk::DeviceSize> offsets = {0};
            commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);
            commandBuffer.bindIndexBuffer(drawInfo.indices->get_handle(), 0, drawInfo.indices->get_index_type());

            commandBuffer.drawIndexed(drawInfo.indices->get_index_count(), 1, 0, 0, 0);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
