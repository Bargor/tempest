// This file is part of Tempest project
// Author: Karol Kontny

#include "draw_info.h"

#include "resources/index_buffer.h"
#include "resources/material.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

namespace tst {
namespace engine {
    namespace vulkan {
        draw_info::draw_info(const vertex_buffer& vertices,
                             const index_buffer* indices,
                             pipeline::pipeline_hash pipeline,
                             const base::view& view,
                             const material& material,
                             const std::vector<const uniform_buffer*>& uniforms)
            : vertices(vertices)
            , indices(indices)
            , pipelineHash(pipeline)
            , viewData(view)
            , meshMaterial(material)
            , pipelineState(nullptr)
            , rebindMask(static_cast<std::uint16_t>(bind_flag_bits::all)) {
            descriptorSets.push_back(uniforms[0]->get_descriptor_set());
        }

        draw_info::draw_info(draw_info&& other) noexcept
            : vertices(other.vertices)
            , indices(other.indices)
            , pipelineHash(other.pipelineHash)
            , viewData(other.viewData)
            , meshMaterial(other.meshMaterial)
            , descriptorSets(std::move(other.descriptorSets))
            , pipelineState(other.pipelineState)
            , rebindMask(other.rebindMask) {
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
