// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <cstdint>
#include <GL/glew.h>

namespace tst {
namespace engine {
    namespace opengl {

        class buffer {
        public:
            struct creation_info {};
        public:
            buffer(std::uint64_t size, void* data, GLenum usage);
            ~buffer();

            buffer(buffer&& other) noexcept;
        private:
            GLuint m_buffer;
            std::uint64_t m_memSize;
        };
    }
} // namespace engine
} // namespace tst