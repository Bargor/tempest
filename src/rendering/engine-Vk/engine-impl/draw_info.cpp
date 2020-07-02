// This file is part of Tempest project
// Author: Karol Kontny

#include "draw_info.h"

namespace tst {
namespace engine {
    namespace vulkan {
        draw_info::draw_info(const vertex_buffer* vertices,
                             const index_buffer<std::uint16_t>* indices,
                             const pipeline& pipeline,
                             const std::vector<const uniform_buffer*>& uniforms,
                             const std::vector<const texture*>&)
            : vertices(vertices), indices(indices), pipelineState(pipeline) {
            descriptorSets.push_back(uniforms[0]->get_descriptor_set());
        }

        draw_info::draw_info(draw_info&& other) noexcept
            : vertices(other.vertices)
            , indices(other.indices)
            , pipelineState(other.pipelineState)
            , descriptorSets(std::move(other.descriptorSets)) {
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
