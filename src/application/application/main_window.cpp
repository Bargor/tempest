// This file is part of Tempest-application project
// Author: Karol Kontny

#include "main_window.h"

#include "glfw_exception.h"

#include <assert.h>
#include <device/monitor.h>
#include <thread/main_thread.h>
#include <thread>

namespace tst {
namespace application {

    main_window::main_window(std::string&& name,
                             event_processor<app_event>& eventProcessor,
                             const core::extent<std::uint32_t>& size,
                             fullscreen_mode windowMode,
                             const device::monitor* monitor,
                             const std::array<window_hint, 31>& hints)
        : glfw_window(std::move(name),
                      eventProcessor,
                      size,
                      windowMode,
                      visible_mode::visible,
                      open_mode::opened,
                      focus_mode::focused,
                      cursor_mode::disable,
                      vsync_mode::vsync_off,
                      monitor,
                      hints) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        glfwMakeContextCurrent(m_windowHandle);
    }

    main_window::~main_window() {
    }

} // namespace application
} // namespace tst
