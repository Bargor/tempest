// This file is part of Tempest-application project
// Author: Karol Kontny

#pragma once

#include <chrono>
#include <variant>

#include "window.h"

namespace tst {
namespace application {

    using objectId = void*;

    struct event {
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
            std::int32_t key;
            std::int32_t scancode;
            std::int32_t action;
            std::int32_t mods;
        };

        struct iconify {
            window::open_option open;
        };

        struct focus {
            window::focus_option focused;
        };

        struct framebuffer {
            window::window_size size;
        };

        struct closed {};

		struct visible {
            window::visible_option visible;
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