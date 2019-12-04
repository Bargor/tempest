// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <GL/glew.h>
#include <vector>

namespace tst {
namespace engine {
    namespace opengl {

        struct draw_elements_command {
            GLuint count;
            GLuint primitivesCount;
            GLuint firstIndex;
            GLint baseVertex;
            GLuint baseInstance;
        };

        struct draw_arrays_command {
            GLuint count;
            GLuint primitivesCount;
            GLuint first;
            GLuint baseInstance;
        };

        class command_list {
        public:
            
        private:
            std::vector<draw_elements_command> m_drawCommands;
        };
    }
}
}