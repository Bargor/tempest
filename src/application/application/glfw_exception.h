// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <stdexcept>
#include <string>

namespace tst {
namespace application {

    class glfw_exception : public std::runtime_error {
        using super = std::runtime_error;
    public:

        explicit glfw_exception(const std::string& message) : super(("Glfw exception: " + message).c_str()) {
        }

        explicit glfw_exception(const char* message) : super(message) {
        }
    };
} // namespace application
} // namespace tst