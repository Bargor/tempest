// This file is part of Tempest project
// Author: Karol Kontny

#include "draw_info.h"

namespace tst {
namespace engine {
    namespace vulkan {
        draw_info::draw_info(const vertex_buffer* vertices,
                             const index_buffer<std::uint16_t>* indices,
                             const pipeline* pipelineState)
            : vertices(vertices), indices(indices), pipelineState(pipelineState) {
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
