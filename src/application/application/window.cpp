// This file is part of Tempest-application project
// Author: Karol Kontny

#include "window.h"

#include <assert.h>

namespace tst {
namespace application {

    window::window(std::string&& name,
                   event_processor<app_event>& eventProcessor,
                   const core::extent<std::uint32_t>& size,
                   fullscreen_mode windowMode,
                   visible_mode visibility,
                   open_mode open,
                   focus_mode focus,
                   cursor_mode cursor)
        : m_name(name)
        , m_eventProcessor(eventProcessor)
        , m_size(size)
        , m_windowMode(windowMode)
        , m_visible(visibility)
        , m_opened(open)
        , m_focused(focus)
        , m_cursor(cursor) {
    }

    window::~window() {
    }

    core::extent<std::uint32_t> window::get_size() const noexcept {
        return m_size;
    }

    float window::get_aspect() const noexcept {
        return static_cast<float>(m_size.width) / m_size.height;
    }

    window::cursor_mode window::get_cursor_mode() const noexcept {
        return m_cursor;
    }

    window::visible_mode window::is_visible() const noexcept {
        return m_visible;
    }

} // namespace application
} // namespace tst
