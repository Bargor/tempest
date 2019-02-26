// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "init.h"

#include <GL/glew.h>

namespace tst {
namespace engine {
    namespace backend {

        void init_backend() {
            glewExperimental = GL_TRUE;
            GLenum err = glewInit();
            if (err != GLEW_OK) {
                fmt::printf("Can't initialize OpenGL, error %s\n", glewGetErrorString(err));
                std::exit(EXIT_FAILURE);
            }
        }
    }
} // namespace engine
} // namespace tst