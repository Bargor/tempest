// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include "glfw_window.h"

namespace tst {

namespace device {
    class monitor;
}

namespace application {

    class main_window final : public glfw_window {
        using super = glfw_window;

    public:
        main_window(std::string&& name,
                    event_processor<app_event>& eventProcessor,
                    const core::extent<std::uint32_t>& size,
                    fullscreen_option windowMode,
                    const device::monitor* monitor,
                    const std::array<window_hint, 31>& hints);
        ~main_window();

    private:
    };
} // namespace application
} // namespace tst
