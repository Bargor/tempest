// This file is part of Tempest project
// Author: Karol Kontny

#include "draw_info.h"

namespace tst {
namespace engine {
    namespace vulkan {
        draw_info::draw_info(const std::string techniqueName,
                             const uniform_buffer_object& transformation,
                             const vertex_buffer* vertices,
                             const index_buffer<std::uint16_t>* indices,
                             std::size_t pipelineHash)
            : base::draw_info(techniqueName)
            , transformation(transformation)
            , vertices(vertices)
            , indices(indices)
            , pipelineHash(pipelineHash) {
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
