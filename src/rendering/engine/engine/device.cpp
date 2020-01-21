// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "device.h"

#include "resource_factory.h"

namespace tst {
namespace engine {
    device::device(application::main_window& mainWindow,
                   application::event_processor<application::app_event>& eventProcessor,
                   settings&& settings)
        : api::device(mainWindow, eventProcessor, std::move(settings)) {
    }

    device::~device() {
    }

    api::resource_factory device::create_resource_factory(const application::data_loader& dataLoader) const {
        return super::create_resource_factory(dataLoader);
    }

} // namespace engine
} // namespace tst
