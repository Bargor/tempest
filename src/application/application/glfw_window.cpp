// This file is part of Tempest-application project
// Author: Karol Kontny

#include "glfw_window.h"

#include "app_event.h"
#include "event_processor.h"
#include "glfw_exception.h"

#include <assert.h>
#include <device/monitor.h>
#include <thread/main_thread.h>
#include <util/variant.h>

namespace tst {
namespace application {

    namespace {
        int translate_cursor_mode(window::cursor_mode mode) {
            switch (mode) {
            case window::cursor_mode::normal:
                return GLFW_CURSOR_NORMAL;
            case window::cursor_mode::hidden:
                return GLFW_CURSOR_HIDDEN;
            case window::cursor_mode::disabled:
                return GLFW_CURSOR_DISABLED;
            }
            throw;
        }
    } // namespace

    glfw_window::glfw_window(std::string&& name,
                             event_processor<app_event>& eventProcessor,
                             const core::extent<std::uint32_t>& size,
                             fullscreen_mode windowMode,
                             visible_mode visibility,
                             open_mode open,
                             focus_mode focus,
                             cursor_mode cursor,
                             vsync_mode vsync,
                             const device::monitor* monitor,
                             const std::array<window_hint, 31>& hints)
        : window(std::move(name), eventProcessor, size, windowMode, visibility, open, focus, cursor)
        , m_vsync(vsync)
        , m_monitor(monitor)
        , m_windowHandle(nullptr) {
        for (std::uint32_t i = 0; i < hints.size(); i++) {
            glfwWindowHint(hints[i].hint, hints[i].value);
        }

        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_windowHandle = (windowMode == fullscreen_mode::fullscreen) ?
            glfwCreateWindow(m_size.width, m_size.height, m_name.c_str(), m_monitor->get_handle(), nullptr) :
            glfwCreateWindow(m_size.width, m_size.height, m_name.c_str(), nullptr, nullptr);
        if (!m_windowHandle) {
            throw glfw_exception("Can't create window");
        }
        glfwSwapInterval(static_cast<int>(vsync));

        auto set_size_callback = [&](const app_event::arguments& args) {
            assert(std::holds_alternative<app_event::framebuffer>(args));
            set_size_internal(std::get<app_event::framebuffer>(args).size, false);
        };
        auto focus_callback = [&](const app_event::arguments& args) {
            if (std::get<app_event::focus>(args).focused == focus_mode::focused) {
                focus_internal(false);
            } else {
                unfocus();
            }
        };
        auto visible_callback = [&](const app_event::arguments& args) {
            if (std::get<app_event::visible>(args).visible == visible_mode::visible) {
                show_internal(false);
            } else {
                hide_internal(false);
            }
        };
        auto iconify_callback = [&](const app_event::arguments& args) {
            if (std::get<app_event::iconify>(args).open == open_mode::iconified) {
                iconify_internal(false);
            } else if (std::get<app_event::iconify>(args).open == open_mode::maximized) {
                maximize_internal(false);
            } else {
                restore_internal(false);
            }
        };
        auto close_callback = [&](const app_event::arguments&) { close_internal(false); };

        glfwSetInputMode(m_windowHandle, GLFW_CURSOR, translate_cursor_mode(cursor));

        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::framebuffer>(), this, std::move(set_size_callback));
        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::focus>(), this, std::move(focus_callback));
        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::visible>(), this, std::move(visible_callback));
        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::iconify>(), this, std::move(iconify_callback));
        m_eventProcessor.subscribe(
            core::variant_index<app_event::arguments, app_event::closed>(), this, std::move(close_callback));
    }

    glfw_window::~glfw_window() {
        close();
    }

    void glfw_window::set_size_internal(const core::extent<std::uint32_t>& size, bool broadcast) noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_size = size;
        if (broadcast) {
            glfwSetWindowSize(m_windowHandle, size.width, size.height);
            m_eventProcessor.create_event(app_event{this, app_event::framebuffer{size.width, size.width}});
        }
    }

    void glfw_window::set_size(const core::extent<std::uint32_t>& size) noexcept {
        set_size_internal(size, false);
    }

    core::position<std::int32_t> glfw_window::get_position() const noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        core::position<std::int32_t> pos;
        glfwGetWindowPos(m_windowHandle, &pos.x, &pos.y);
        return pos;
    }

    void glfw_window::set_position(const core::position<std::int32_t>& pos) noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        assert(m_windowMode == fullscreen_mode::windowed);
        glfwSetWindowPos(m_windowHandle, pos.x, pos.y);
    }

    void glfw_window::set_title(std::string name) noexcept {
        assert(m_windowHandle);
        glfwSetWindowTitle(m_windowHandle, name.c_str());
        m_name = std::move(name);
    }

    void glfw_window::set_cursor_mode(cursor_mode mode) noexcept {
        assert(m_windowHandle);
        m_cursor = mode;
        glfwSetInputMode(m_windowHandle, GLFW_CURSOR, translate_cursor_mode(mode));
        m_eventProcessor.create_event(app_event{this, app_event::cursor{m_cursor}});
    }

    void glfw_window::focus_internal(bool broadcast) noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        // assert(m_focused == focus_option::unfocused);
        m_focused = focus_mode::focused;
        if (broadcast) {
            glfwFocusWindow(m_windowHandle);
            m_eventProcessor.create_event(app_event{this, app_event::focus{m_focused}});
        }
    }

    void glfw_window::focus() noexcept {
        focus_internal(true);
    }

    void glfw_window::unfocus() noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        assert(m_focused == focus_mode::focused);
        m_focused = focus_mode::unfocused;
    }

    void glfw_window::show_internal(bool broadcast) noexcept {
        assert(m_windowHandle);
        assert(m_visible == visible_mode::hidden);
        assert(m_windowMode == fullscreen_mode::windowed);
        m_visible = visible_mode::visible;
        if (broadcast) {
            glfwShowWindow(m_windowHandle);
            m_eventProcessor.create_event(app_event{this, app_event::visible{m_visible}});
        }
    }

    void glfw_window::show() noexcept {
        show_internal(true);
    }

    void glfw_window::hide_internal(bool broadcast) noexcept {
        assert(m_windowHandle);
        assert(m_visible == visible_mode::visible);
        assert(m_windowMode == fullscreen_mode::windowed);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_visible = visible_mode::hidden;
        if (broadcast) {
            glfwHideWindow(m_windowHandle);
            m_eventProcessor.create_event(app_event{this, app_event::visible{m_visible}});
        }
    }

    void glfw_window::hide() noexcept {
        hide_internal(true);
    }

    void glfw_window::iconify_internal(bool broadcast) noexcept {
        assert(m_windowHandle);
        assert(m_opened != open_mode::iconified);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_opened = open_mode::iconified;
        if (broadcast) {
            glfwIconifyWindow(m_windowHandle);
            m_eventProcessor.create_event(app_event{this, app_event::iconify{m_opened}});
        }
    }

    void glfw_window::iconify() noexcept {
        iconify_internal(true);
    }

    void glfw_window::restore_internal(bool broadcast) noexcept {
        assert(m_windowHandle);
        assert(m_opened != open_mode::opened);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_opened = open_mode::opened;
        if (broadcast) {
            glfwRestoreWindow(m_windowHandle);
            m_eventProcessor.create_event(app_event{this, app_event::iconify{m_opened}});
        }
    }

    void glfw_window::restore() noexcept {
        restore_internal(false);
    }

    void glfw_window::maximize() noexcept {
        maximize_internal(true);
    }

    void glfw_window::maximize_internal(bool broadcast) noexcept {
        assert(m_opened != open_mode::maximized);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_opened = open_mode::maximized;
        if (broadcast) {
            glfwMaximizeWindow(m_windowHandle);
            m_eventProcessor.create_event(app_event{this, app_event::iconify{m_opened}});
        }
    }

    void glfw_window::close() noexcept {
        close_internal(true);
    }

    void glfw_window::close_internal(bool broadcast) noexcept {
        if (m_windowHandle) {
            assert(std::this_thread::get_id() == core::main_thread::get_id());
            if (broadcast) {
                glfwDestroyWindow(m_windowHandle);
                m_windowHandle = nullptr;
                m_eventProcessor.create_event(app_event{this, app_event::closed{}});
            }
        }
    }

    glfw_window::vsync_mode glfw_window::get_vsync() const noexcept {
        return m_vsync;
    }

    void glfw_window::set_vsync(vsync_mode option) noexcept {
        glfwSwapInterval(static_cast<int>(option));
        m_vsync = option;
    }

    void glfw_window::end_frame() noexcept {
        glfwSwapBuffers(m_windowHandle);
    }

    GLFWwindow* glfw_window::get_handle() const noexcept {
        return m_windowHandle;
    }

} // namespace application
} // namespace tst
