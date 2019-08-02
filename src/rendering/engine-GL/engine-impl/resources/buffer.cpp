// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "buffer.h"

namespace tst {
namespace engine {
    namespace opengl {
        buffer::buffer(std::uint64_t size) : m_memSize(size) {
            glCreateBuffers(1, &m_buffer);
        }

        buffer::~buffer() {
            glDeleteBuffers(1, &m_buffer);
        }

        buffer::buffer(buffer&& other) noexcept : m_buffer(other.m_buffer) {
            other.m_buffer = 0;
        }
    }
}
}