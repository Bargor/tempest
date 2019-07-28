// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "engine.h"

#include <engine-impl/engine.h>

namespace tst {
namespace engine {

    rendering_engine::rendering_engine(application::main_window& mainWindow,
                                       application::data_loader& dataLoader,
                                       application::event_processor<application::app_event>& eventProcessor)
        : m_engineImpl(mainWindow, dataLoader, eventProcessor), m_device(m_engineImpl.get_GPU()) {
    }

    rendering_engine::~rendering_engine() {
    }

    void rendering_engine::frame(size_t frameCount) {
            m_engineImpl.frame(frameCount);
    }

    void rendering_engine::start() {
        m_engineImpl.start();
    }

    void rendering_engine::stop() {
        m_engineImpl.stop();
    }

    device& rendering_engine::get_GPU() noexcept {
        return m_device;
    }

    void rendering_engine::submit_draw_commands(std::vector<draw_command>& drawCommands) {
        
    }

} // namespace engine
} // namespace tst