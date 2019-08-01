// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <GL/glew.h>

namespace tst {
namespace engine {
    namespace opengl {
        class buffer {
        public:
            buffer(std::uint32_t size);
            ~buffer();

            buffer(buffer&& other) noexcept;
        private:
            GLuint m_buffer;
        };
    }
} // namespace engine
} // namespace tst