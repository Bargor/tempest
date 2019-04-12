// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#pragma once

#include <application/main_window.h>

#include <array>

namespace tst {
namespace application {

    std::array<main_window::window_hint, 31> setup_context(const std::int32_t refreshRate);

}
}