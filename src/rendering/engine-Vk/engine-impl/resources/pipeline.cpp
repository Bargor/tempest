// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "pipeline.h"

#include "vertex_format.h"

namespace tst {
namespace engine {
    namespace vulkan {

        pipeline::pipeline(const vk::Device logicalDevice,
                           base::pipeline&& base,
                           const vertex_format& format,
                           const shader_set& shaders,
                           const rendering_technique& technique)
            : super(std::move(base)) {
        }

        pipeline::~pipeline() {
        }

        void pipeline::bind_command_buffer(vk::CommandBuffer& buffer, vk::PipelineBindPoint& bindPoint) {
            buffer.bindPipeline(bindPoint, m_pipeline);
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
