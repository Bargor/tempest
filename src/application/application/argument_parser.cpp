// This file is part of Tempest-application project
// Author: Karol Kontny

#include "argument_parser.h"

#include <string>
#include <vector>

namespace tst {
namespace application {

    program_params parse_arguments(const int argc, const char** argv) {
        program_params programParams;
        for (int i = 0; i < argc; i++) {
            std::string param(argv[i]);

            if (param == "-w") {
                programParams.window_mode = main_window::fullscreen_option::windowed;
                continue;
            }

            if (param == "-f") {
                programParams.window_mode = main_window::fullscreen_option::fullscreen;
                continue;
            }
        }
        return programParams;
    }
} // namespace application
} // namespace tst