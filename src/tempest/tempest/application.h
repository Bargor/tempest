// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#pragma once

#include <core.h>
#include <thread>
#include <string>

namespace tst {

namespace device {
    class monitor;
}

namespace application {

    class main_window;
	template<typename Event>
    class event_processor;
    class input_processor;
    class simulation_engine;
    class data_loader;
    class time_source;
    struct app_event;
    struct program_params;

    class application {

    public:
        application(const program_params& params, ptr<device::monitor> monitor, std::string&& name);
        ~application();
        void run();

    private:
        ptr<device::monitor> m_monitor;
        ptr<time_source> m_timeSource;
        ptr<event_processor<app_event>> m_eventProcessor;
        ptr<main_window> m_mainWindow;
        ptr<input_processor> m_inputProcessor;
        ptr<data_loader> m_dataLoader;
        ptr<simulation_engine> m_engine;
    };

} // namespace application
} // namespace tst