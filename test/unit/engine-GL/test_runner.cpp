// This file is part of Tempest-engine-Gl project
// Author: Karol Kontny

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <fmt/printf.h>
#include <gtest/gtest.h>

#ifdef NDEBUG
#define OPENGL_DEBUG GLFW_FALSE
#else
#define OPENGL_DEBUG GLFW_TRUE
#endif

struct window_hint {
    std::int32_t hint;
    std::int32_t value;
};

std::array<window_hint, 31> setup_context(const std::int32_t refreshRate) {
    std::array<window_hint, 31> hints = {{{GLFW_RESIZABLE, GLFW_TRUE},
                                          {GLFW_VISIBLE, GLFW_FALSE},
                                          {GLFW_DECORATED, GLFW_TRUE},
                                          {GLFW_FOCUSED, GLFW_TRUE},
                                          {GLFW_AUTO_ICONIFY, GLFW_TRUE},
                                          {GLFW_FLOATING, GLFW_FALSE},
                                          {GLFW_MAXIMIZED, GLFW_FALSE},
                                          {GLFW_RED_BITS, 8},
                                          {GLFW_BLUE_BITS, 8},
                                          {GLFW_GREEN_BITS, 8},
                                          {GLFW_ALPHA_BITS, 8},
                                          {GLFW_DEPTH_BITS, 24},
                                          {GLFW_ACCUM_RED_BITS, 0},
                                          {GLFW_ACCUM_GREEN_BITS, 0},
                                          {GLFW_ACCUM_BLUE_BITS, 0},
                                          {GLFW_ACCUM_ALPHA_BITS, 0},
                                          {GLFW_AUX_BUFFERS, 0},
                                          {GLFW_SAMPLES, 0},
                                          {GLFW_REFRESH_RATE, refreshRate},
                                          {GLFW_STEREO, GLFW_FALSE},
                                          {GLFW_SRGB_CAPABLE, GLFW_TRUE},
                                          {GLFW_DOUBLEBUFFER, GLFW_TRUE},
                                          {GLFW_CLIENT_API, GLFW_OPENGL_API},
                                          {GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API},
                                          {GLFW_CONTEXT_VERSION_MAJOR, 3},
                                          {GLFW_CONTEXT_VERSION_MINOR, 3},
                                          {GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS},
                                          {GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR},
                                          {GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE},
                                          {GLFW_OPENGL_DEBUG_CONTEXT, OPENGL_DEBUG},
                                          {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE}}};

    return hints;
}

GLFWwindow* create_context() {
    auto hints = setup_context(60);

    for (std::uint32_t i = 0; i < hints.size(); i++) {
        glfwWindowHint(hints[i].hint, hints[i].value);
    }

    auto window = glfwCreateWindow(500, 500, "Tempest test", nullptr, nullptr);
    if (!window) {
        fmt::printf("Can't create window");
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    return window;
}

void init_Glfw() {
    if (!glfwInit()) {
        fmt::printf("Can't initialize Glfw\n");
        std::exit(EXIT_FAILURE);
    }
}

void deinit_Glfw() {
    glfwTerminate();
}

void init_OpenGL() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fmt::printf("Can't initialize OpenGL, error %s\n",
                    std::string_view(reinterpret_cast<const char*>(glewGetErrorString(err))));
        std::exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    init_Glfw();
    auto window = create_context();
    init_OpenGL();
    auto res = RUN_ALL_TESTS();
    glfwDestroyWindow(window);
    deinit_Glfw();
    return res;
}
