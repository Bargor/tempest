// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

#include "gpu_info.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <application/main_window.h>

namespace tst {
namespace engine {
    namespace opengl {

        device::device(application::main_window&,
                       application::event_processor<application::app_event>&,
                       settings&&)
            : m_gpuInfo(std::make_unique<gpu_info>()) {
        }

        device::~device() {
        }

        void device::start() {
        }

        void device::stop() {
        }

        vertex_buffer device::create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) const {
            return vertex_buffer(format, std::move(vertices));
        }

        uniform_buffer device::create_uniform_buffer() const {
            return uniform_buffer();
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
