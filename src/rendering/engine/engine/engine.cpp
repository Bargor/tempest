// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include <engine-impl/engine.h>

namespace tst {
namespace engine {

    rendering_engine::rendering_engine(application::data_loader& dataLoader,
                                       application::event_processor<application::app_event>& eventProcessor,
                                       device& device)
        : api::rendering_engine(dataLoader, eventProcessor, device) {
    }

    rendering_engine::~rendering_engine() {
    }

    void rendering_engine::start() {
        super::start();
    }

    void rendering_engine::stop() {
        super::stop();
    }

} // namespace engine
} // namespace tst
