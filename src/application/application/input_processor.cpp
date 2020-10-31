// This file is part of Tempest-application project
// Author: Karol Kontny

#include "input_processor.h"

#include "glfw_exception.h"
#include "glfw_window.h"

#include <GLFW/glfw3.h>
#include <assert.h>
#include <thread/main_thread.h>

namespace tst {
namespace application {

    input_processor::input_processor(event_processor<app_event>& event_processor, const glfw_window& window)
        : m_eventProcessor(event_processor), m_window(window), m_lastMousePosition({0, 0}) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        const auto glfwWindow = m_window.get_handle();
        glfwSetWindowUserPointer(glfwWindow, this);
        glfwGetCursorPos(glfwWindow, &m_lastMousePosition.xpos, &m_lastMousePosition.ypos);

        auto focus_callback = [](GLFWwindow* window, const std::int32_t focused) {
            static_cast<input_processor*>(glfwGetWindowUserPointer(window))->window_focus_callback(window, focused);
        };
        auto cursor_callback = [](GLFWwindow* window, const double xpos, const double ypos) {
            static_cast<input_processor*>(glfwGetWindowUserPointer(window))->cursor_pos_callback(window, xpos, ypos);
        };
        auto mouse_button_callback =
            [](GLFWwindow* window, const std::int32_t button, const std::int32_t action, const std::int32_t mods) {
                static_cast<input_processor*>(glfwGetWindowUserPointer(window))
                    ->mouse_button_callback(window, button, action, mods);
            };
        auto scroll_callback = [](GLFWwindow* window, const double xoffset, const double yoffset) {
            static_cast<input_processor*>(glfwGetWindowUserPointer(window))->mouse_scroll_callback(window, xoffset, yoffset);
        };
        auto key_callback = [](GLFWwindow* window,
                               const std::int32_t key,
                               const std::int32_t scancode,
                               const std::int32_t action,
                               const std::int32_t mods) {
            static_cast<input_processor*>(glfwGetWindowUserPointer(window))->key_callback(window, key, scancode, action, mods);
        };
        auto iconify_callback = [](GLFWwindow* window, const std::int32_t iconified) {
            static_cast<input_processor*>(glfwGetWindowUserPointer(window))->window_iconify_callback(window, iconified);
        };
        auto close_callback = [](GLFWwindow* window) {
            static_cast<input_processor*>(glfwGetWindowUserPointer(window))->window_close_callback(window);
        };
        auto framebuffer_size_callback = [](GLFWwindow* window, const std::int32_t width, const std::int32_t height) {
            static_cast<input_processor*>(glfwGetWindowUserPointer(window))
                ->framebuffer_size_callback(window,
                                            {static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)});
        };

        if (glfwSetWindowFocusCallback(glfwWindow, focus_callback) != nullptr) {
            throw glfw_exception("Can't set focus callback");
        }
        if (glfwSetCursorPosCallback(glfwWindow, cursor_callback) != nullptr) {
            throw glfw_exception("Can't set cursor callback");
        }
        if (glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback) != nullptr) {
            throw glfw_exception("Can't set mouse button callback");
        }
        if (glfwSetScrollCallback(glfwWindow, scroll_callback) != nullptr) {
            throw glfw_exception("Can't set mouse scroll callback");
        }
        if (glfwSetKeyCallback(glfwWindow, key_callback) != nullptr) {
            throw glfw_exception("Can't set key callback");
        }
        if (glfwSetWindowIconifyCallback(glfwWindow, iconify_callback) != nullptr) {
            throw glfw_exception("Can't set window iconify callback");
        }
        if (glfwSetWindowCloseCallback(glfwWindow, close_callback) != nullptr) {
            throw glfw_exception("Can't set window close callback");
        }
        if (glfwSetFramebufferSizeCallback(glfwWindow, framebuffer_size_callback) != nullptr) {
            throw glfw_exception("Can't set framebuffer size callback");
        }
    }

    void input_processor::process_events() {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        glfwPollEvents();
    }

    void input_processor::window_focus_callback(GLFWwindow*, const std::int32_t focused) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_eventProcessor.create_event(app_event{this, app_event::focus{static_cast<window::focus_mode>(focused)}});
    }

    void input_processor::cursor_pos_callback(GLFWwindow*, double xpos, double ypos) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        const auto newMousePos = [this](double xpos, double ypos) {
            return m_window.get_cursor_mode() == window::cursor_mode::disabled ?
                app_event::mouse_pos{xpos - m_lastMousePosition.xpos, ypos - m_lastMousePosition.ypos} :
                app_event::mouse_pos{xpos, ypos};
        }(xpos, ypos);
        m_lastMousePosition = app_event::mouse_pos{xpos, ypos};

        m_eventProcessor.create_event(app_event{this, newMousePos});
    }

    void input_processor::mouse_button_callback(GLFWwindow*, std::int32_t button, std::int32_t action, std::int32_t mods) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_eventProcessor.create_event(app_event{this,
                                                app_event::mouse_button{static_cast<device::mouse_buttons>(button),
                                                                        static_cast<device::mouse_action>(action),
                                                                        mods}});
    }

    void input_processor::mouse_scroll_callback(GLFWwindow*, double xoffset, double yoffset) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_eventProcessor.create_event(app_event{this, app_event::scroll{xoffset, yoffset}});
    }

    void input_processor::key_callback(GLFWwindow*,
                                       const std::int32_t key,
                                       const std::int32_t scancode,
                                       const std::int32_t action,
                                       const std::int32_t mods) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_eventProcessor.create_event(app_event{
            this,
            app_event::keyboard{static_cast<device::keys>(key), scancode, static_cast<device::key_action>(action), mods}});
    }

    void input_processor::window_iconify_callback(GLFWwindow*, const std::int32_t iconified) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_eventProcessor.create_event(app_event{this, app_event::iconify{static_cast<window::open_mode>(iconified)}});
    }

    void input_processor::window_close_callback(GLFWwindow*) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_eventProcessor.create_event(app_event{this, app_event::closed{}});
    }

    void input_processor::framebuffer_size_callback(GLFWwindow*, const core::extent<std::uint32_t>& windowSize) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_eventProcessor.create_event(app_event{this, app_event::framebuffer{windowSize}});
    }
} // namespace application
} // namespace tst
