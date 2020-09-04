// This file is part of Tempest-application project
// Author: Karol Kontny

#include "argument_parser.h"

#include <string_view>
#include <vector>

namespace tst {
namespace application {

    program_params parse_arguments(const int argc, const char** argv) {

		const auto executionDirectory = std::filesystem::path(argv[0]).parent_path().append("");

        for (int i = 1; i < argc; i++) {
            std::string_view param(argv[i]);

            if (param == "-w") {
                return {main_window::fullscreen_mode::windowed, executionDirectory};
            }

            if (param == "-f") {
                return {main_window::fullscreen_mode::fullscreen, executionDirectory};
            }
        }
        return {main_window::fullscreen_mode::windowed, executionDirectory};
    }
} // namespace application
} // namespace tst