// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

#include "application/main_window.h"
#include "gpu_info.h"

namespace tst {
namespace engine {
    namespace opengl {

        device::device(application::main_window&) : m_gpuInfo(std::make_unique<gpu_info>()) {
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

        bool device::draw() {
            return true;
        }
    }
}
}