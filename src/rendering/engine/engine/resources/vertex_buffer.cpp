// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

namespace tst {
namespace engine {
    namespace resources {
        vertex_buffer::vertex_buffer(api::vertex_buffer&& bufferImpl) : api::vertex_buffer(std::move(bufferImpl)) {
        }

        vertex_buffer::~vertex_buffer() {
        }

        vertex_buffer::vertex_buffer(vertex_buffer&& other) noexcept : api::vertex_buffer(std::move(other)) {
        }
    } // namespace resources
} // namespace engine
} // namespace tst