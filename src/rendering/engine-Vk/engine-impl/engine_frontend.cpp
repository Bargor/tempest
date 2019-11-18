// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine_frontend.h"

#include "device.h"
#include "draw_info.h"
#include "instance.h"
#include "resource_cache.h"
#include "resources/index_buffer.h"
#include "resources/rendering_technique.h"
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

        engine_frontend::engine_frontend(application::event_processor<application::app_event>& eventProcessor,
                                         device& device,
                                         resource_cache& resourceCache)
            : m_eventProcessor(eventProcessor)
            , m_device(device)
            , m_resourceCache(resourceCache)
            , m_commandPools({m_device.create_command_pool(), m_device.create_command_pool()})
            , m_bufferCache(m_commandPools.size()) {
        }

        engine_frontend::~engine_frontend() {
            for (int i = 0; i < m_commandPools.size(); i++) {
                m_device.m_logicalDevice.freeCommandBuffers(
                    m_commandPools[i], static_cast<std::uint32_t>(m_bufferCache[i].size()), m_bufferCache[i].data());
            }
        }

        vk::CommandBuffer engine_frontend::generate_command_buffer(const draw_info& drawInfo) {
            auto idx = m_device.get_resource_index();
            vk::CommandBufferAllocateInfo bufferAllocateInfo(
                m_commandPools[idx], vk::CommandBufferLevel::ePrimary, 1);

            m_device.m_logicalDevice.resetCommandPool(m_commandPools[idx],
                                                      vk::CommandPoolResetFlags());

            if (m_bufferCache[idx].size() == 0) {
                m_bufferCache[idx].emplace_back(
                    m_device.m_logicalDevice.allocateCommandBuffers(bufferAllocateInfo)[0]);
            }

            auto commandBuffer = m_bufferCache[m_device.get_resource_index()][0];

            vk::CommandBufferBeginInfo commandBufferInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr);
            commandBuffer.begin(commandBufferInfo);
            commandBuffer.beginRenderPass(drawInfo.pipelineState.get_technique().generate_render_pass_info(),
                                          vk::SubpassContents::eInline);

            drawInfo.pipelineState.bind_command_buffer(commandBuffer, vk::PipelineBindPoint::eGraphics);

            std::vector<vk::Buffer> vertexBuffers = {drawInfo.vertices->get_handle()};
            std::vector<vk::DeviceSize> offsets = {0};
            commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);
            commandBuffer.bindIndexBuffer(drawInfo.indices->get_handle(), 0, vk::IndexType::eUint16);
            commandBuffer.drawIndexed(drawInfo.indices->get_index_count(), 1, 0, 0, 0);
            commandBuffer.endRenderPass();
            commandBuffer.end();

            return commandBuffer;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
