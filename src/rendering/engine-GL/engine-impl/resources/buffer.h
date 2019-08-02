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
            buffer(std::uint64_t size);
            ~buffer();

            buffer(buffer&& other) noexcept;
        private:
            GLuint m_buffer;
            std::uint64_t m_memSize;
        };
    }
} // namespace engine
} // namespace tst