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
                fmt::printf("Can't initialize OpenGL, error %s\n", std::string_view(glewGetErrorString(err)));
                std::exit(EXIT_FAILURE);
            }
        }

        engine_frontend::engine_frontend(application::data_loader& dataLoader,
                                         application::event_processor<application::app_event>&,
                                         device& device)
            : m_dataLoader(dataLoader), m_device(device) {
            init_OpenGL();
        }

        engine_frontend::~engine_frontend() {
        }

        void engine_frontend::frame(size_t) {
        }

    } // namespace opengl
} // namespace engine
} // namespace tst
