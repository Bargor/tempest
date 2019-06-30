// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include <memory>

namespace tst {
namespace application {
    class data_loader;
    class main_window;
	template<typename Event>
    class event_processor;
    struct app_event;
} // namespace application

namespace scene {
    class scene;
}

namespace engine {

    class rendering_engine {
        template<typename T>
        using ptr = std::unique_ptr<T>;

    public:
        rendering_engine(application::main_window&,
                         application::data_loader& dataLoader,
                         application::event_processor<app_event>&);
        ~rendering_engine();

        void frame(size_t frameCount);
        void start();
        void stop();

    private:
        application::data_loader& m_dataLoader;
        ptr<scene::scene> m_scene;
    };

} // namespace engine
} // namespace tst