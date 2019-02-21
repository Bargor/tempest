// This file is part of Tempest-application project
// Author: Karol Kontny

#include "glfw_window.h"

#include "glfw_exception.h"

#include <thread/main_thread.h>
#include <device/monitor.h>
#include <thread>
#include <assert.h>

namespace tst {
namespace application {

    glfw_window::glfw_window(std::string&& name,
                             const window_size& size,
                             fullscreen_option windowMode,
                             visible_option visibility,
                             open_option open,
                             focus_option focus,
                             vsync_option vsync,
                             const device::monitor* monitor,
                             const std::array<window_hint, 31>& hints)
        : window(std::move(name), size, windowMode, visibility, open, focus)
        , m_vsync(vsync)
        , m_monitor(monitor)
        , m_windowHandle(nullptr) {
        for (std::uint32_t i = 0; i < hints.size(); i++) {
            glfwWindowHint(hints[i].hint, hints[i].value);
        }

        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_windowHandle = (windowMode == fullscreen_option::fullscreen) ?
            glfwCreateWindow(m_size.width, m_size.height, m_name.c_str(), m_monitor->get_handle(), nullptr) :
            glfwCreateWindow(m_size.width, m_size.height, m_name.c_str(), nullptr, nullptr);
        if (!m_windowHandle) { throw glfw_exception("Can't create window"); }
        glfwSwapInterval(static_cast<int>(vsync));
    }

    glfw_window::~glfw_window() {
        if (m_windowHandle) {
            assert(std::this_thread::get_id() == core::main_thread::get_id());
            glfwDestroyWindow(m_windowHandle);
            m_windowHandle = nullptr;
        }
    }

    void glfw_window::set_size(const window_size& size) noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        glfwSetWindowSize(m_windowHandle, size.width, size.height);
        m_size = size;
    }

    position<std::int32_t> glfw_window::get_position() const noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        position<std::int32_t> pos;
        glfwGetWindowPos(m_windowHandle, &pos.x, &pos.y);
        return pos;
    }

    void glfw_window::set_position(const position<std::int32_t>& pos) noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        assert(m_windowMode == fullscreen_option::windowed);
        glfwSetWindowPos(m_windowHandle, pos.x, pos.y);
    }

    void glfw_window::set_title(const std::string& name) noexcept {
        assert(m_windowHandle);
        glfwSetWindowTitle(m_windowHandle, name.c_str());
        m_name = name;
    }

    void glfw_window::focus() noexcept {
        assert(m_windowHandle);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        assert(m_focused == focus_option::unfocused);
        glfwFocusWindow(m_windowHandle);
        m_focused = focus_option::focused;
    }

    void glfw_window::show() noexcept {
        assert(m_windowHandle);
        assert(m_visible == visible_option::hidden);
        assert(m_windowMode == fullscreen_option::windowed);
        glfwShowWindow(m_windowHandle);
        m_visible = visible_option::visible;
    }

    void glfw_window::hide() noexcept {
        assert(m_windowHandle);
        assert(m_visible == visible_option::visible);
        assert(m_windowMode == fullscreen_option::windowed);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        glfwHideWindow(m_windowHandle);
        m_visible = visible_option::hidden;
    }

    void glfw_window::iconify() noexcept {
        assert(m_windowHandle);
        assert(m_opened != open_option::iconified);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        glfwIconifyWindow(m_windowHandle);
        m_opened = open_option::iconified;
    }

    void glfw_window::restore() noexcept {
        assert(m_windowHandle);
        assert(m_opened != open_option::opened);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_opened = open_option::opened;
        glfwRestoreWindow(m_windowHandle);
    }

    void glfw_window::maximize() noexcept {
        assert(m_opened != open_option::maximized);
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        m_opened = open_option::maximized;
        glfwMaximizeWindow(m_windowHandle);
    }

    void glfw_window::close() noexcept {
        if (m_windowHandle) {
            assert(std::this_thread::get_id() == core::main_thread::get_id());
            glfwDestroyWindow(m_windowHandle);
            m_windowHandle = nullptr;
        }
    }

    glfw_window::vsync_option glfw_window::get_vsync() const noexcept { return m_vsync; }

    void glfw_window::set_vsync(vsync_option option) noexcept {
        glfwSwapInterval(static_cast<int>(option));
        m_vsync = option;
    }

    void glfw_window::end_frame() noexcept { glfwSwapBuffers(m_windowHandle); }

    GLFWwindow* glfw_window::get_handle() const noexcept { return m_windowHandle; }

} // namespace application
} // namespace tst
