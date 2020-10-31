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
                       const application::data_loader&,
                       settings&&)
            : m_gpuInfo(std::make_unique<gpu_info>()) {
        }

        device::~device() {
        }

        bool device::start_frame() {
            return true;
        }

        void device::start() {
        }

        void device::stop() {
        }

        resource_factory device::create_resource_factory(const application::data_loader& dataLoader) const {
            return resource_factory(dataLoader);
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
