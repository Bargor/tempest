// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "application.h"

#include "context.h"
#include "engine.h"

#include <application/app_event.h>
#include <application/argument_parser.h>
#include <application/data_loader.h>
#include <application/event_processor.h>
#include <application/input_processor.h>
#include <device/monitor.h>

namespace tst {
namespace application {

    application::application(const program_params& params, ptr<device::monitor> monitor, std::string&& name)
        : m_monitor(std::move(monitor))
        , m_timeSource(std::make_unique<time_source>())
        , m_eventProcessor(std::make_unique<event_processor<app_event>>(*m_timeSource.get()))
        , m_mainWindow(std::make_unique<main_window>(
              std::move(name),
              *m_eventProcessor.get(),
              main_window::window_size{m_monitor->get_width() / 2, m_monitor->get_height() / 2},
              params.windowMode,
              m_monitor.get(),
              setup_context(m_monitor->get_refresh_rate())))
        , m_inputProcessor(std::make_unique<input_processor>(*m_eventProcessor.get(), *m_mainWindow.get()))
        , m_dataLoader(
              std::make_unique<data_loader>(std::initializer_list<std::filesystem::path>({params.executionDirectory})))
        , m_engine(std::make_unique<simulation_engine>(*m_timeSource.get(),
                                                       *m_eventProcessor.get(),
                                                       *m_inputProcessor.get(),
                                                       *m_mainWindow.get(),
                                                       *m_dataLoader.get())) {
    }

    application::~application() {
    }

    void application::run() {
        m_engine->run();
    }

} // namespace application
} // namespace tst
