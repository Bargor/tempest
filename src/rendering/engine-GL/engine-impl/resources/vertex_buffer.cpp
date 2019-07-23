

#include "vertex_buffer.h"

namespace tst {
namespace engine {
    namespace opengl {

        vertex_buffer::vertex_buffer(const vertex_format&,
                                     std::vector<vertex>&& vertices)
            : m_vertices(std::move(vertices)) {
        }

        vertex_buffer::~vertex_buffer() {
        }

        vertex_buffer::vertex_buffer(vertex_buffer&& other) noexcept
            : m_format(other.m_format), m_vertices(std::move(other.m_vertices)) {
        }

    }
}
}