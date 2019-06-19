// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <array>
#include <common/position.h>
#include <string>

namespace tst {

namespace device {
    class monitor;
}

namespace application {

    class event_processor;

    class window {
    public:
        enum class fullscreen_option { fullscreen, windowed };

        enum class visible_option { hidden, visible };

        enum class open_option { opened = 0, iconified = 1, maximized = 2 };

        enum class focus_option { unfocused, focused };

        struct window_size {
            std::int32_t width;
            std::int32_t height;
        };

    public:
        visible_option is_visible() const noexcept;
        window_size get_size() const noexcept;
        virtual void set_size(const window_size& size) = 0;
        virtual position<std::int32_t> get_position() const noexcept = 0;
        virtual void set_position(const position<std::int32_t>& pos) noexcept = 0;
        virtual void set_title(const std::string&) noexcept = 0;
        virtual void focus() noexcept = 0;
        virtual void unfocus() noexcept = 0;
        virtual void show() noexcept = 0;
        virtual void hide() noexcept = 0;
        virtual void iconify() noexcept = 0;
        virtual void restore() noexcept = 0;
        virtual void maximize() noexcept = 0;
        virtual void close() noexcept = 0;

    protected:
        window(std::string&& name,
               event_processor& m_eventProcessor,
               const window_size& size,
               fullscreen_option windowMode,
               visible_option visibility,
               open_option open,
               focus_option focus);
        virtual ~window() = 0;

    protected:
        std::string m_name;
        event_processor& m_eventProcessor;

        window_size m_size;
        fullscreen_option m_windowMode;
        visible_option m_visible;
        open_option m_opened;
        focus_option m_focused;
    };

} // namespace application
} // namespace tst