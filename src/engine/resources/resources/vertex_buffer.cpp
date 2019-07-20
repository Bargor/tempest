// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "vertex_buffer.h"

namespace tst {
namespace engine {
    namespace resources {
        vertex_buffer::vertex_buffer(api::vertex_buffer&& nativeBuffer)
            : m_nativeVertexBuffer(std::move(nativeBuffer)) {
        }

        vertex_buffer::~vertex_buffer() {
        }

        vertex_buffer::vertex_buffer(vertex_buffer&& other) noexcept : m_nativeVertexBuffer(std::move(other.m_nativeVertexBuffer)) {
        }
    } // namespace resources
} // namespace engine
} // namespace tst