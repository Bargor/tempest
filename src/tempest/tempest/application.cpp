// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "application.h"
#include "engine.h"
#include "context.h"

#include <application/data_loader.h>
#include <application/argument_parser.h>
#include <application/event_processor.h>
#include <application/input_processor.h>

#include <device/monitor.h>

namespace tst {
namespace application {

    application::application(const program_params& params, ptr<device::monitor> monitor, std::string&& name)
        : m_eventProcessor(std::make_unique<event_processor>())
        , m_mainWindow(std::make_unique<main_window>(
              std::move(name),
              *m_eventProcessor.get(),
              main_window::window_size{monitor->get_width() / 2, monitor->get_height() / 2},
              params.windowMode,
              monitor.get(),
              setup_context(monitor->get_refresh_rate())))
        , m_monitor(std::move(monitor))
        , m_inputProcessor(std::make_unique<input_processor>(*m_eventProcessor.get(), *m_mainWindow.get()))
        , m_dataLoader(
              std::make_unique<data_loader>(std::initializer_list<std::filesystem::path>({params.executionDirectory})))
        , m_engine(std::make_unique<simulation_engine>(
              *m_eventProcessor.get(), *m_inputProcessor.get(), *m_mainWindow.get(), *m_dataLoader.get())) {}

    application::~application() {}

    void application::run() { m_engine->run(); }

} // namespace application
} // namespace tst
