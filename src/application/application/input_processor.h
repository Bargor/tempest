// This file is part of Tempest-application project
// Author: Karol Kontny

#pragma once

#include "event_processor.h"

#include <common/rectangle.h>

struct GLFWwindow;

namespace tst {
namespace application {

    class glfw_window;

    struct app_event;

    class input_processor {
    public:
        input_processor(event_processor<app_event>& event_processor, const glfw_window& window);

        void process_events();

    private:
        void window_focus_callback(GLFWwindow*, const std::int32_t focused);
        void cursor_pos_callback(GLFWwindow*, const double xpos, const double ypos);
        void mouse_button_callback(GLFWwindow*,
                                   const std::int32_t button,
                                   const std::int32_t action,
                                   const std::int32_t mods);
        void mouse_scroll_callback(GLFWwindow*, const double xoffset, const double yoffset);
        void key_callback(GLFWwindow*,
                          const std::int32_t key,
                          const std::int32_t scancode,
                          const std::int32_t action,
                          const std::int32_t mods);
        void window_iconify_callback(GLFWwindow*, const std::int32_t iconified);
        void window_close_callback(GLFWwindow*);
        void framebuffer_size_callback(GLFWwindow*, const core::extent<std::uint32_t>& windowSize);

        event_processor<app_event>& m_eventProcessor;
        GLFWwindow* const m_window;
    };

} // namespace application
} // namespace tst
