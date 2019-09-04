// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

namespace tst {
namespace engine {
    device::device(application::main_window& mainWindow,
                   application::event_processor<application::app_event>& eventProcessor)
        : api::device(mainWindow, eventProcessor) {
    }

    device::~device() {
    }

} // namespace engine
} // namespace tst
