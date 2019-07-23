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

namespace engine {
	
	namespace opengl {

    class device;

    class rendering_engine {
        template<typename T>
        using ptr = std::unique_ptr<T>;

    public:
        rendering_engine(application::main_window&,
                         application::data_loader& dataLoader,
                         application::event_processor<application::app_event>&);
        ~rendering_engine();

        void frame(size_t frameCount);
        void start();
        void stop();
		device& get_GPU() const noexcept;

    private:
        application::data_loader& m_dataLoader;
        ptr<device> m_device;
    };
	
	}
} // namespace engine
} // namespace tst