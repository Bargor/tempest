// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include <engine/api.h>
#include <memory>
#include <time/timer.h>

namespace tst {

namespace engine {
    class rendering_engine;

    namespace resources {
        class resource_factory;
    }
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
    class time_source;

    struct app_event;

    class simulation_engine {
        template<typename T>
        using ptr = std::unique_ptr<T>;

    public:
        simulation_engine(time_source& timeSource,
                          event_processor<app_event>& eventProcessor,
                          input_processor& inputProcessor,
                          main_window& mainWindow,
                          data_loader& dataLoader);
        ~simulation_engine();

        void run();

    private:
        void main_loop();

    private:
        time_source& m_timeSource;
        event_processor<app_event>& m_eventProcessor;
        input_processor& m_inputProcessor;
        main_window& m_mainWindow;
        data_loader& m_dataLoader;
        ptr<engine::api::rendering_engine> m_renderingEngine;
        engine::api::device& m_renderingDevice;
        ptr<engine::resources::resource_factory> m_resourceFactory;
        ptr<scene::scene> m_scene;
        std::size_t m_frameCounter;
        std::uint32_t m_lastSecondFrames;
        bool m_shouldClose;
        bool m_windowMinimized;
        std::chrono::duration<std::uint64_t, std::micro> m_lastFrameDuration;
    };

} // namespace application
} // namespace tst