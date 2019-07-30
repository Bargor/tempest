// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "draw_item.h"

namespace tst {
namespace engine {
    namespace opengl {
        draw_info::draw_info(const uniform_buffer_object& transformation,
                             const vertex_buffer* vertices,
                             const index_buffer<std::uint16_t>* indices)
            : m_transformation(transformation), m_vertices(vertices), m_indices(indices) {
        }
    }
}
}