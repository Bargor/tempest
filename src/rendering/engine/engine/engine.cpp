// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include <engine-impl/engine.h>

namespace tst {
namespace engine {

    rendering_engine::rendering_engine(application::main_window& mainWindow,
                                       application::data_loader& dataLoader,
                                       application::event_processor<application::app_event>& eventProcessor)
        : api::rendering_engine(mainWindow, dataLoader, eventProcessor), m_device(super::get_GPU()) {
    }

    rendering_engine::~rendering_engine() {
    }

    void rendering_engine::frame(size_t frameCount) {
        super::frame(frameCount);
    }

    void rendering_engine::start() {
        super::start();
    }

    void rendering_engine::stop() {
        super::stop();
    }

    device& rendering_engine::get_GPU() noexcept {
        return m_device;
    }

} // namespace engine
} // namespace tst