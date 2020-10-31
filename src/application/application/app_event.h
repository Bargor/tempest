// This file is part of Tempest-application project
// Author: Karol Kontny

#pragma once

#include "object_id.h"
#include "window.h"

#include <chrono>
#include <device/keyboard.h>
#include <device/mouse.h>
#include <variant>

namespace tst {
namespace application {

    struct app_event {
        struct mouse_pos {
            double xpos;
            double ypos;
        };

        struct mouse_button {
            device::mouse_buttons button;
            device::mouse_action action;
            std::int32_t mods;
        };

        struct scroll {
            double xoffset;
            double yoffset;
        };

        struct keyboard {
            device::keys key;
            std::int32_t scancode;
            device::key_action action;
            std::int32_t mods;
        };

        struct iconify {
            window::open_mode open;
        };

        struct focus {
            window::focus_mode focused;
        };

        struct framebuffer {
            core::extent<std::uint32_t> size;
        };

        struct closed {};

        struct visible {
            window::visible_mode visible;
        };

        struct time {
            std::chrono::microseconds time;
        };

        struct cursor {
            window::cursor_mode cursor;
        };

        using arguments =
            std::variant<mouse_pos, mouse_button, cursor, scroll, keyboard, focus, iconify, closed, visible, time, framebuffer>;

        objectId id;
        arguments args;
    };

} // namespace application
} // namespace tst
