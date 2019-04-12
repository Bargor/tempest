// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "engine.h"

#include <application/event_processor.h>
#include <application/input_processor.h>
#include <application/main_window.h>
#include <engine/engine.h>
#include <util/variant.h>

namespace tst {
namespace application {

    simulation_engine::simulation_engine(event_processor& eventProcessor,
                                         input_processor& inputProcessor,
                                         main_window& mainWindow,
                                         data_loader& dataLoader)
        : m_eventProcessor(eventProcessor)
        , m_inputProcessor(inputProcessor)
        , m_mainWindow(mainWindow)
        , m_dataLoader(dataLoader)
        , m_renderingEngine(std::make_unique<engine::rendering_engine>(m_dataLoader, mainWindow, eventProcessor))
        , m_timer()
        , m_frameCount(0)
        , m_shouldClose(false) {
        auto close_callback = [&](const event::arguments&) { m_shouldClose = true; };
        m_eventProcessor.subscribe(core::variant_index<event::arguments, event::closed>(), std::move(close_callback));
    }

    simulation_engine::~simulation_engine() {
    }

    void simulation_engine::run() {
        auto lastFrameTime = std::chrono::microseconds::zero();
        while (!m_shouldClose) {
            m_timer.start();
            main_loop();
            m_timer.stop();

            lastFrameTime = m_timer.get_time();
            m_timer.reset();
            m_eventProcessor.create_event(event{event::time{lastFrameTime}});
            m_frameCount++;
        }
        m_renderingEngine->stop();
    }

    void simulation_engine::main_loop() {
        m_inputProcessor.process_events();
        m_eventProcessor.process_events();
        m_renderingEngine->frame(m_frameCount);
        m_mainWindow.end_frame();
    }

} // namespace application
} // namespace tst