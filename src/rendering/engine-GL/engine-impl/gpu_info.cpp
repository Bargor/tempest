// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "gpu_info.h"

#include <GL/glew.h>

namespace tst {
namespace engine {
    namespace opengl {
        gpu_info::gpu_info() {
            GLint major, minor, patch = 0;
            GLuint buffer;
            glGenBuffers(1, &buffer);

            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);

            versionMajor = major;
            versionMinor = minor;
            versionPatch = patch;

            std::string vendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            std::string renderer(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
            std::string version(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
        }
    }
}
}