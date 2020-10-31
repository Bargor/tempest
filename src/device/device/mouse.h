// This file is part of Tempest-device project
// Author: Karol Kontny

#pragma once
#include <GLFW/glfw3.h>
#include <cstdint>

namespace tst {
namespace device {

    enum class mouse_buttons : std::uint8_t {
        button_left = GLFW_MOUSE_BUTTON_LEFT,
        button_right = GLFW_MOUSE_BUTTON_RIGHT,
        button_middle = GLFW_MOUSE_BUTTON_MIDDLE
    };

    enum class mouse_action : std::uint8_t { release = GLFW_RELEASE, press = GLFW_PRESS, repeat = GLFW_REPEAT };
} // namespace device
} // namespace tst