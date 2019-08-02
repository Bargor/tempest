

#include "vertex_buffer.h"

namespace tst {
namespace engine {
    namespace opengl {

        vertex_buffer::vertex_buffer(const vertex_format&,
                                     std::vector<vertex>&& vertices) : buffer(vertices.size() * sizeof(vertex))
            , m_vertices(std::move(vertices)) {
        }

        vertex_buffer::~vertex_buffer() {
        }

        vertex_buffer::vertex_buffer(vertex_buffer&& other) noexcept
            : buffer(std::move(other)), m_format(other.m_format), m_vertices(std::move(other.m_vertices)) {
        }

    }
}
}