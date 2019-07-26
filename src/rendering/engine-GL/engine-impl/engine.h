// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include <memory>

namespace tst {
namespace application {
    struct app_event;
    class data_loader;
    template<typename Event>
    class event_processor;
    class main_window;
} // namespace application

namespace engine {
	
	namespace opengl {

        class device;
        class shader_compiler;

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
            ptr<shader_compiler> m_shaderCompiler;
        };
	
	}
} // namespace engine
} // namespace tst