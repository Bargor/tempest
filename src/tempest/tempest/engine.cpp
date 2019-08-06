// This file is part of Tempest project
// Author: Karol Kontny

#include "engine.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <application/input_processor.h>
#include <engine/engine.h>
#include <application/main_window.h>
#include <fmt/printf.h>
#include <engine/resource_factory.h>
#include <scene/scene.h>
#include <util/variant.h>

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
        , m_renderingDevice(std::make_unique<engine::device>(m_mainWindow))
        , m_renderingEngine(std::make_unique<engine::rendering_engine>(m_mainWindow, m_dataLoader, m_eventProcessor, *m_renderingDevice))
        , m_resourceFactory(std::make_unique<engine::resource_factory>(*m_renderingDevice))
        , m_scene(std::make_unique<scene::scene>())
        , m_frameCounter(0)
        , m_lastSecondFrames(0)
        , m_shouldClose(false)
        , m_windowMinimized(false) {
        auto close_callback = [&](const app_event::arguments&) { m_shouldClose = true; };
        auto iconify_callback = [&](const app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::iconify>(args));
            m_windowMinimized =
                std::get<application::app_event::iconify>(args).open == window::open_option::iconified ? true : false;
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

        std::vector<engine::resources::vertex_buffer> vertexBuffers;
        vertexBuffers.emplace_back(
            m_resourceFactory->create_vertex_buffer(engine::vertex_format{},
                                                    std::vector<engine::vertex>({{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                                                 {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                                                 {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                                                 {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}})));
        std::vector<engine::resources::index_buffer> indexBuffers;
        indexBuffers.emplace_back(
            m_resourceFactory->create_index_buffer(std::vector<std::uint16_t>({{0, 1, 2, 2, 3, 0}})));

        scene::scene_object object(std::move(vertexBuffers[0]), std::move(indexBuffers[0]));
        m_scene->add_object(std::move(object));
    }

    simulation_engine::~simulation_engine() {
    }

    void simulation_engine::run() {
        while (!m_shouldClose) {
            main_loop();
            m_frameCounter++;
        }
        m_renderingEngine->stop();
    }

    void simulation_engine::main_loop() {
        m_inputProcessor.process_events();
        m_eventProcessor.process_events();
        auto frameStart = m_timeSource.now();
        if (!m_windowMinimized) {
            auto newSceneState = scene::update_scene(*m_scene, m_lastFrameDuration);
            auto drawInfo = scene::prepare_draw_info(newSceneState);
            m_renderingEngine->draw_frame(drawInfo.begin(), drawInfo.end());
            m_renderingEngine->frame(m_frameCounter);
            m_mainWindow.end_frame();
            m_lastSecondFrames++;
        }
        m_lastFrameDuration = frameStart - m_timeSource.now();
    }

} // namespace application
} // namespace tst