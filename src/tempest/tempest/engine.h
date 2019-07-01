// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include <memory>
#include <time/timer.h>

namespace tst {

namespace engine {
    class rendering_engine;
}

namespace scene {
    class scene;
}

namespace application {

	template<typename Event>
    class event_processor;
    class input_processor;
    class main_window;
    class data_loader;

	struct app_event;

    class simulation_engine {
        template<typename T>
        using ptr = std::unique_ptr<T>;

    public:
        simulation_engine(event_processor<app_event>& eventProcessor,
                          input_processor& inputProcessor,
                          main_window& mainWindow,
                          data_loader& dataLoader);
        ~simulation_engine();

        void run();

    private:
        void main_loop();

    private:
        event_processor<app_event>& m_eventProcessor;
        input_processor& m_inputProcessor;
        main_window& m_mainWindow;
        data_loader& m_dataLoader;
        ptr<scene::scene> m_scene;
        ptr<engine::rendering_engine> m_renderingEngine;
        std::size_t m_frameCount;
        bool m_shouldClose;
        bool m_windowMinimized;
        std::chrono::microseconds m_lastSecondTimer;
        std::uint32_t m_lastSecondFrames;
    };

} // namespace application
} // namespace tst