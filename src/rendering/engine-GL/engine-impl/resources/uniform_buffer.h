// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "buffer.h"

#include <glm.h>

namespace tst {
namespace engine {
    namespace opengl {
		
        class uniform_buffer : public buffer {
        public:
            uniform_buffer(const std::size_t storageSize);
            
            uniform_buffer(uniform_buffer&& other) noexcept;
            uniform_buffer(const uniform_buffer& other) = delete;

            ~uniform_buffer();

            void update_buffer(const void* data, const std::size_t dataSize);
        private:
		};
	}
}
}