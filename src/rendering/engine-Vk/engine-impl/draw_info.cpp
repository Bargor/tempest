// This file is part of Tempest project
// Author: Karol Kontny

#include "draw_info.h"

#include "resources/material.h"
#include "resources/pipeline.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

namespace tst {
namespace engine {
    namespace vulkan {
        draw_info::draw_info(const vertex_buffer* vertices,
                             const index_buffer<std::uint16_t>* indices,
                             const pipeline& pipeline,
                             const material& material,
                             const std::vector<const uniform_buffer*>& uniforms)
            : vertices(vertices), indices(indices), pipelineState(pipeline), meshMaterial(material) {
            descriptorSets.push_back(uniforms[0]->get_descriptor_set());
        }

        draw_info::draw_info(draw_info&& other) noexcept
            : vertices(other.vertices)
            , indices(other.indices)
            , pipelineState(other.pipelineState)
            , meshMaterial(other.meshMaterial)
            , descriptorSets(std::move(other.descriptorSets)) {
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
