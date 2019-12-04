// This file is part of Tempest-application project
// Author: Karol Kontny

#pragma once

#include "main_window.h"

#include <vector>
#include <filesystem>

namespace tst {
namespace application {

    struct program_params {
        main_window::fullscreen_option windowMode = main_window::fullscreen_option::windowed;
        std::filesystem::path executionDirectory;
    };

    program_params parse_arguments(const int argc, const char** argv);
} // namespace application
} // namespace tst