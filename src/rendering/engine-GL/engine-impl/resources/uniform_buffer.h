// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <glm/glm.hpp>

namespace tst {
namespace engine {
    namespace opengl {
		
		struct uniform_buffer_object {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };
		
        class uniform_buffer {
            uniform_buffer();
            ~uniform_buffer();
		};
	}
}
}