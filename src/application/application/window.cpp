// This file is part of Tempest-application project
// Author: Karol Kontny

#include "window.h"

#include <assert.h>

namespace tst {
namespace application {

    window::window(std::string&& name,
                   event_processor<app_event>& eventProcessor,
                   const core::extent<std::uint32_t>& size,
                   fullscreen_option windowMode,
                   visible_option visibility,
                   open_option open,
                   focus_option focus)
        : m_name(name)
        , m_eventProcessor(eventProcessor)
        , m_size(size)
        , m_windowMode(windowMode)
        , m_visible(visibility)
        , m_opened(open)
        , m_focused(focus) {
    }

    window::~window() {
    }

    core::extent<std::uint32_t> window::get_size() const noexcept {
        return m_size;
    }

    window::visible_option window::is_visible() const noexcept {
        return m_visible;
    }

} // namespace application
} // namespace tst
