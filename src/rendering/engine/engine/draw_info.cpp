// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "draw_info.h"

namespace tst {
namespace engine {
    draw_info::draw_info(const api::uniform_buffer_object& transformation,
                         const resources::vertex_buffer* vertices,
                         const resources::index_buffer* indices)
        : api::draw_info(transformation, vertices, indices) {
    }

} // namespace engine
} // namespace tst