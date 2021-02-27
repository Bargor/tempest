// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "test_environment.h"

#include <array>
#include <engine-impl/instance.h>
#include <fmt/printf.h>
#include <gtest/gtest.h>
#include <vulkan/vulkan.hpp>

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
                                          {GLFW_VISIBLE, GLFW_TRUE},
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
                                          {GLFW_CLIENT_API, GLFW_NO_API},
                                          {GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API},
                                          {GLFW_CONTEXT_VERSION_MAJOR, 3},
                                          {GLFW_CONTEXT_VERSION_MINOR, 3},
                                          {GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS},
                                          {GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR},
                                          {GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE},
                                          {GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE},
                                          {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE}}};

    return hints;
}

GLFWwindow* create_context() {
    auto hints = setup_context(60);

    for (std::uint32_t i = 0; i < hints.size(); i++) {
        glfwWindowHint(hints[i].hint, hints[i].value);
    }

    g_glfwWindow = glfwCreateWindow(500, 500, "Tempest test", nullptr, nullptr);
    if (!g_glfwWindow) {
        fmt::printf("Can't create window");
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(g_glfwWindow);
    return g_glfwWindow;
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

int main(int argc, char** argv) {
    g_executionDirectory = std::filesystem::path(argv[0]).parent_path().append("");
    ::testing::InitGoogleTest(&argc, argv);
    init_Glfw();
    tst::engine::vulkan::instance::enableGlfwExtensions = false;
    auto window = create_context();
    auto res = RUN_ALL_TESTS();
    glfwDestroyWindow(window);
    deinit_Glfw();
    return res;
}
