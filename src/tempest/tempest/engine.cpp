// This file is part of Tempest project
// Author: Karol Kontny

#include "engine.h"

// clang-format off
#include <engine/device.h>
#include <application/app_event.h>
#include <application/event_processor.h>
#include <application/input_processor.h>
#include <application/main_window.h>
#include <engine/resource_factory.h>
#include <engine/settings.h>
#include <scene/scene.h>
#include <scene/object_controller.h>
#include <util/variant.h>

#include <fmt/printf.h>
// clang-format on

namespace tst {
namespace application {

    simulation_engine::simulation_engine(time_source& timeSource,
                                         event_processor<app_event>& eventProcessor,
                                         input_processor& inputProcessor,
                                         main_window& mainWindow,
                                         data_loader& dataLoader)
        : m_timeSource(timeSource)
        , m_eventProcessor(eventProcessor)
        , m_inputProcessor(inputProcessor)
        , m_mainWindow(mainWindow)
        , m_dataLoader(dataLoader)
        , m_renderingDevice(std::make_unique<engine::device>(
              m_mainWindow, m_eventProcessor, engine::api::parse_engine_settings(dataLoader)))
        , m_resourceFactory(std::make_unique<engine::resource_factory>(*m_renderingDevice, m_dataLoader))
        , m_scene(std::make_unique<scene::scene>("world", dataLoader, eventProcessor, *m_resourceFactory))
        , m_frameCounter(0)
        , m_lastSecondFrames(0)
        , m_shouldClose(false)
        , m_windowMinimized(false)
        , m_lastFrameDuration(0) {
        auto close_callback = [&](const app_event::arguments&) { m_shouldClose = true; };
        auto iconify_callback = [&](const app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::iconify>(args));
            m_windowMinimized =
                std::get<application::app_event::iconify>(args).open == window::open_mode::iconified ? true : false;
        };
        auto time_callback = [&](const app_event::arguments&) {
            fmt::printf("FPS: %d\n", m_lastSecondFrames);
            m_lastSecondFrames = 0;
        };
        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::closed>(), this, std::move(close_callback));
        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::iconify>(), this, std::move(iconify_callback));
        m_eventProcessor.subscribe(core::variant_index<app_event::arguments, app_event::time>(),
                                   this,
                                   std::move(time_callback),
                                   std::chrono::seconds(1));
        m_scene->add_object("test", "test");
        m_scene->add_camera("main",
                            glm::vec3(0.0f, 0.0f, 5.0f),
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f),
                            90.0f,
                            m_mainWindow.get_aspect());
    }

    simulation_engine::~simulation_engine() {
    }

    void simulation_engine::run() {
        m_renderingDevice->start();
        while (!m_shouldClose) {
            main_loop();
            m_frameCounter++;
        }
        m_renderingDevice->stop();
    }

    void simulation_engine::main_loop() {
        m_inputProcessor.process_events();
        m_eventProcessor.process_events();
        const auto frameStart = m_timeSource.now();
        if (!m_windowMinimized) {
            const auto newSceneState = scene::update_scene(*m_scene, m_lastFrameDuration);
            const auto drawInfo = scene::prepare_draw_info(m_scene->get_camera("main"), newSceneState);
            m_renderingDevice->draw_frame(drawInfo.begin(), drawInfo.end());
            m_mainWindow.end_frame();
            m_lastSecondFrames++;
        }
        m_lastFrameDuration = m_timeSource.now() - frameStart;
    }

} // namespace application
} // namespace tst
