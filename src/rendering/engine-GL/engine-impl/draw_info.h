// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

namespace tst {
namespace engine {
    namespace opengl {
        class draw_info {
            public:
            draw_info(const uniform_buffer_object& transformation,
                      const vertex_buffer* vertices,
                      const index_buffer<std::uint16_t>* indices);
        private:
            const uniform_buffer_object& m_transformation;
            const vertex_buffer* m_vertices;
            const index_buffer<std::uint16_t>* m_indices;
        };
    }
}
}