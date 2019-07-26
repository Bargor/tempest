// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

namespace tst {
namespace engine {
    namespace opengl {

        device::device() {

        }

        device::~device() {
            
        }

        vertex_buffer device::create_vertex_buffer(const vertex_format& format,
                                                   std::vector<vertex>&& vertices) const {
            return vertex_buffer(format, std::move(vertices));
        }

        uniform_buffer device::create_uniform_buffer() const {
            return uniform_buffer();
        }
    }
}
}