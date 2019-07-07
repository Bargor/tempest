// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace tst {
namespace engine {
	namespace resources {

		struct vertex {
            glm::vec2 pos;
            glm::vec3 color;
        };

        class vertex_format {
        public:
            vertex_format();

        private:
        };
	}

}
}