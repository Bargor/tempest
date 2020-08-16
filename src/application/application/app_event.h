// This file is part of Tempest-application project
// Author: Karol Kontny

#pragma once

#include "object_id.h"
#include "window.h"

#include <chrono>
#include <variant>

#include <device/keyboard.h>

namespace tst {
namespace application {

    struct app_event {
        struct mouse_pos {
            double xpos;
            double ypos;
        };

        struct mouse_button {
            std::int32_t button;
            std::int32_t action;
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

        using arguments =
            std::variant<mouse_pos, mouse_button, scroll, keyboard, focus, iconify, closed, visible, time, framebuffer>;

        objectId id;
        arguments args;
    };

} // namespace application
} // namespace tst
