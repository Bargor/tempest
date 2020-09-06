// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

#include "vertex_format.h"

namespace tst {
namespace engine {
    namespace resources {

        vertex_buffer::vertex_buffer(const creation_info& info, const vertex_format& format, std::vector<vertex>&& vertices)
            : api::vertex_buffer(info, format.to_super(), std::move(vertices)) {
        }

        vertex_buffer::~vertex_buffer() {
        }

        vertex_buffer::vertex_buffer(vertex_buffer&& other) noexcept : api::vertex_buffer(std::move(other)) {
        }
    } // namespace resources
} // namespace engine
} // namespace tst