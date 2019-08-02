// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "buffer.h"

namespace tst {
namespace engine {
    namespace opengl {
        buffer::buffer(std::uint64_t size, void* data, GLenum usage) : m_memSize(size) {
            glCreateBuffers(1, &m_buffer);
            glNamedBufferData(m_buffer, m_memSize, data, usage);
        }

        buffer::~buffer() {
            glDeleteBuffers(1, &m_buffer);
        }

        buffer::buffer(buffer&& other) noexcept : m_buffer(other.m_buffer), m_memSize(other.m_memSize) {
            other.m_buffer = 0;
        }
    }
}
}