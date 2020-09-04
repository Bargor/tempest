// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <array>
#include <common/position.h>
#include <common/rectangle.h>
#include <string>

namespace tst {

namespace device {
    class monitor;
}

namespace application {

    template<typename Event>
    class event_processor;
    struct app_event;

    class window {
    public:
        enum class fullscreen_mode { fullscreen, windowed };

        enum class visible_mode { hidden, visible };

        enum class open_mode { opened = 0, iconified = 1, maximized = 2 };

        enum class focus_mode { unfocused, focused };

        enum class cursor_mode { normal, hidden, disabled };

    public:
        visible_mode is_visible() const noexcept;
        core::extent<std::uint32_t> get_size() const noexcept;
        float get_aspect() const noexcept;
        cursor_mode get_cursor_mode() const noexcept;
        virtual void set_size(const core::extent<std::uint32_t>& size) = 0;

        virtual core::position<std::int32_t> get_position() const noexcept = 0;
        virtual void set_position(const core::position<std::int32_t>& pos) noexcept = 0;
        virtual void set_title(std::string) noexcept = 0;
        virtual void set_cursor_mode(cursor_mode cursor) noexcept = 0;
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
               event_processor<app_event>& m_eventProcessor,
               const core::extent<std::uint32_t>& size,
               fullscreen_mode windowMode,
               visible_mode visibility,
               open_mode open,
               focus_mode focus,
               cursor_mode cursor);
        virtual ~window() = 0;

    protected:
        std::string m_name;
        event_processor<app_event>& m_eventProcessor;

        core::extent<std::uint32_t> m_size;
        fullscreen_mode m_windowMode;
        visible_mode m_visible;
        open_mode m_opened;
        focus_mode m_focused;
        cursor_mode m_cursor;
    };

} // namespace application
} // namespace tst
