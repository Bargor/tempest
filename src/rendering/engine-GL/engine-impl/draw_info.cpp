// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "draw_info.h"

namespace tst {
namespace engine {
    namespace opengl {
        draw_info::draw_info(const vertex_buffer* vertices,
                             const index_buffer<std::uint16_t>* indices,
                             const pipeline& pipeline,
                             const std::vector<const uniform_buffer*>& uniforms,
                             const std::vector<const texture*>& texture)
            : vertices(vertices), indices(indices), pipelineState(pipeline), uniforms(uniforms), textures(texture) {
        }
    } // namespace opengl
} // namespace engine
} // namespace tst