// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#pragma once

#include <memory>
#include <thread>

namespace tst {

namespace device {
    class monitor;
}

namespace application {

    class main_window;
    class event_processor;
    class input_processor;
    class simulation_engine;
    class data_loader;
    struct program_params;

    class application {
        template<typename T>
        using ptr = std::unique_ptr<T>;

    public:
        application(const program_params& params, ptr<device::monitor> monitor, std::string&& name);
        ~application();
        void run();

    private:
        ptr<main_window> m_mainWindow;
        ptr<device::monitor> m_monitor;
        ptr<event_processor> m_eventProcessor;
        ptr<input_processor> m_inputProcessor;
        ptr<data_loader> m_dataLoader;
        ptr<simulation_engine> m_engine;
    };

} // namespace application
} // namespace tst