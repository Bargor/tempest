// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "context.h"

#ifdef NDEBUG
#define OPENGL_DEBUG GLFW_FALSE
#else
#define OPENGL_DEBUG GLFW_TRUE
#endif

namespace tst {
namespace application {

    std::array<main_window::window_hint, 31> setup_context(const std::int32_t refreshRate) {
        std::array<main_window::window_hint, 31> hints = {{{GLFW_RESIZABLE, GLFW_TRUE},
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
                                                           {GLFW_CLIENT_API, GLFW_OPENGL_API},
                                                           {GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API},
                                                           {GLFW_CONTEXT_VERSION_MAJOR, 4},
                                                           {GLFW_CONTEXT_VERSION_MINOR, 5},
                                                           {GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS},
                                                           {GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR},
                                                           {GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE},
                                                           {GLFW_OPENGL_DEBUG_CONTEXT, OPENGL_DEBUG},
                                                           {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE}}};

        return hints;
    }
} // namespace application
} // namespace tst