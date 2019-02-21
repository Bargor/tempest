// This file is part of Tempest-application project
// Author: Karol Kontny

#pragma once

#include <chrono>
#include <variant>

namespace tst {
namespace application {

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
            std::int32_t iconified;
        };

        struct focus {
            std::int32_t focused;
        };

        struct closed {};

        struct time {
            std::chrono::microseconds time;
        };

        using arguments = std::variant<mouse_pos, mouse_button, scroll, keyboard, focus, iconify, closed, time>;

        arguments args;
    };

} // namespace application
} // namespace tst