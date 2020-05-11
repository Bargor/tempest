// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"

#include <glm/glm.h>

namespace tst {
namespace engine {
    namespace opengl {
		
		struct uniform_buffer_object {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };
		
        class uniform_buffer : public buffer {
        public:
            uniform_buffer();
            
            uniform_buffer(uniform_buffer&& other) noexcept;
            uniform_buffer(const uniform_buffer& other) = delete;

            ~uniform_buffer();

            void update_buffer(const uniform_buffer_object& ubo);
        private:
		};
	}
}
}