// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "uniform_buffer.h"

namespace tst {
namespace engine {
    namespace opengl {
        uniform_buffer::uniform_buffer() : buffer(sizeof(uniform_buffer_object)) {        
        }

        uniform_buffer::~uniform_buffer() {          
        }

        uniform_buffer::uniform_buffer(uniform_buffer&& other) noexcept : buffer(std::move(other)) {

        }
    }
}
}