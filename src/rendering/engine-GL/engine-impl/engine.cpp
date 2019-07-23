// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "engine.h"
#include "device.h"

#include <GL/glew.h>
#include <application/app_event.h>
#include <application/event_processor.h>
#include <fmt/printf.h>

namespace tst {
namespace engine {
	namespace opengl {

    void init_OpenGL() {
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            fmt::printf("Can't initialize OpenGL, error %s\n", glewGetErrorString(err));
            std::exit(EXIT_FAILURE);
        }
    }

    rendering_engine::rendering_engine(application::main_window&,
                                       application::data_loader& dataLoader,
                                       application::event_processor<application::app_event>&)
        : m_dataLoader(dataLoader)
        , m_device(std::make_unique<device>()) {
        init_OpenGL();
    }

    rendering_engine::~rendering_engine() {
    }

    void rendering_engine::frame(size_t) {
    }

    void rendering_engine::start() {
    }

    void rendering_engine::stop() {
    }
	
	device& rendering_engine::get_GPU() const noexcept {
            return *m_device;
    }

	}
} // namespace engine
} // namespace tst