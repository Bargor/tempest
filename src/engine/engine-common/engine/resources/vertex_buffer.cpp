// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

namespace tst {
namespace engine {
    namespace resources {
        vertex_buffer::vertex_buffer(api::vertex_buffer&& bufferImpl) : m_vertexBufferImpl(std::move(bufferImpl)) {
        }

        vertex_buffer::~vertex_buffer() {
        }

        vertex_buffer::vertex_buffer(vertex_buffer&& other) noexcept
            : m_vertexBufferImpl(std::move(other.m_vertexBufferImpl)) {
        }
    } // namespace resources
} // namespace engine
} // namespace tst