// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "engine.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <application/input_processor.h>
#include <application/main_window.h>
#include <engine/engine.h>
#include <fmt/printf.h>
#include <scene/scene.h>
#include <util/variant.h>

namespace tst {
namespace application {

    simulation_engine::simulation_engine(event_processor<app_event>& eventProcessor,
                                         input_processor& inputProcessor,
                                         main_window& mainWindow,
                                         data_loader& dataLoader)
        : m_eventProcessor(eventProcessor)
        , m_inputProcessor(inputProcessor)
        , m_mainWindow(mainWindow)
        , m_dataLoader(dataLoader)
        , m_scene(std::make_unique<scene::scene>())
        , m_renderingEngine(std::make_unique<engine::rendering_engine>(mainWindow, m_dataLoader, eventProcessor))
        , m_timer()
        , m_frameCount(0)
        , m_shouldClose(false)
        , m_windowMinimized(false)
        , m_lastSecondTimer(std::chrono::microseconds::zero())
        , m_lastSecondFrames(0) {
        auto close_callback = [&](const app_event::arguments&) { m_shouldClose = true; };
        auto iconify_callback = [&](const app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::iconify>(args));
            m_windowMinimized =
                std::get<application::app_event::iconify>(args).open == window::open_option::iconified ? true : false;
        };
        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::closed>(), this, std::move(close_callback));
        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::iconify>(), this, std::move(iconify_callback));
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
            m_eventProcessor.create_event(app_event{this, app_event::time{lastFrameTime}});
            m_frameCount++;
            m_lastSecondTimer += lastFrameTime;
            m_lastSecondFrames++;
            if (m_lastSecondTimer > std::chrono::seconds(1)) {
                fmt::printf("FPS: %d\n", m_lastSecondFrames);
                m_lastSecondFrames = 0;
                m_lastSecondTimer = std::chrono::microseconds::zero();
            }
        }
        m_renderingEngine->stop();
    }

    void simulation_engine::main_loop() {
        m_inputProcessor.process_events();
        m_eventProcessor.process_events();
        if (!m_windowMinimized) {
            m_renderingEngine->frame(m_frameCount);
            m_mainWindow.end_frame();
        }
    }

} // namespace application
} // namespace tst