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
            , m_commandPools({m_device.create_command_pool(), m_device.create_command_pool()}) {
        }

        engine_frontend::~engine_frontend() {
        }

        vk::CommandBuffer engine_frontend::generate_command_buffer(const draw_info& drawInfo) {
            vk::CommandBufferAllocateInfo bufferAllocateInfo(
                m_commandPools[m_device.get_resource_index()], vk::CommandBufferLevel::ePrimary, 1);

            auto commandBuffers = m_device.m_logicalDevice.allocateCommandBuffers(bufferAllocateInfo);

            vk::CommandBufferBeginInfo commandBufferInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr);
            commandBuffers[0].begin(commandBufferInfo);
            commandBuffers[0].beginRenderPass(drawInfo.pipelineState->get_technique().generate_render_pass_info(),
                                              vk::SubpassContents::eInline);

            drawInfo.pipelineState->bind_command_buffer(commandBuffers[0], vk::PipelineBindPoint::eGraphics);

            std::vector<vk::Buffer> vertexBuffers = {drawInfo.vertices->get_handle()};
            std::vector<vk::DeviceSize> offsets = {0};
            commandBuffers[0].bindVertexBuffers(0, vertexBuffers, offsets);
            commandBuffers[0].bindIndexBuffer(drawInfo.indices->get_handle(), 0, vk::IndexType::eUint16);
            commandBuffers[0].drawIndexed(drawInfo.indices->get_index_count(), 1, 0, 0, 0);
            commandBuffers[0].endRenderPass();
            commandBuffers[0].end();

            return commandBuffers[0];
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
