// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "engine.h"

#include <scene/scene.h>

#include <fmt/printf.h>
#include <GL/glew.h>

namespace tst {
namespace engine {

    void init_OpenGL() {
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            fmt::printf("Can't initialize OpenGL, error %s\n", glewGetErrorString(err));
            std::exit(EXIT_FAILURE);
        }
    }

    rendering_engine::rendering_engine(application::data_loader& dataLoader, application::main_window&)
        : m_dataLoader(dataLoader), m_scene(std::make_unique<scene::scene>()) {
        init_OpenGL();
    }

    rendering_engine::~rendering_engine() {
    }
	
	void rendering_engine::stop() {}

    void rendering_engine::frame(size_t) {
    }

} // namespace engine
} // namespace tst