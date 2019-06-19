// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include "window.h"

#include <GLFW/glfw3.h>

namespace tst {
namespace application {

    class glfw_window : public window {
        using super = window;

    public:
        enum class vsync_option { vsync_off = 0, vsync_on = 1 };

        struct window_hint {
            std::int32_t hint;
            std::int32_t value;
        };

    public: // overrides
        void set_size(const window_size& size) noexcept override;
        position<std::int32_t> get_position() const noexcept override;
        void set_position(const position<std::int32_t>& pos) noexcept override;
        void set_title(const std::string&) noexcept override;
        void focus() noexcept override;
        void unfocus() noexcept override;
        void show() noexcept override;
        void hide() noexcept override;
        void iconify() noexcept override;
        void restore() noexcept override;
        void maximize() noexcept override;
        void close() noexcept override;

    public: // new fuctions
        vsync_option get_vsync() const noexcept;
        void set_vsync(vsync_option option) noexcept;
        void end_frame() noexcept;
        GLFWwindow* get_handle() const noexcept;

    protected:
        glfw_window(std::string&& name,
                    event_processor& eventProcessor,
                    const window_size& size,
                    fullscreen_option windowMode,
                    visible_option visibility,
                    open_option open,
                    focus_option focus,
                    vsync_option vsync,
                    const device::monitor* monitor,
                    const std::array<window_hint, 31>& hints);
        virtual ~glfw_window();

        vsync_option m_vsync;
        const device::monitor* m_monitor;
        GLFWwindow* m_windowHandle;

	private:
        void set_size_internal(const window_size& size, bool broadcast) noexcept;
        void focus_internal(bool broadcast) noexcept;
        void show_internal(bool broadcast) noexcept;
        void hide_internal(bool broadcast) noexcept;
        void iconify_internal(bool broadcast) noexcept;
        void restore_internal(bool broadcast) noexcept;
        void maximize_internal(bool broadcast) noexcept;
        void close_internal(bool broadcast) noexcept;
    };

} // namespace application
} // namespace tst
