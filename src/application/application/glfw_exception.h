// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <stdexcept>
#include <string>

namespace tst {
namespace application {

    class glfw_exception : public std::runtime_error {
    public:
        using super = std::runtime_error;

        explicit glfw_exception(const std::string& message) : super(message.c_str()) {}

        explicit glfw_exception(const char* message) : super(message) {}
    };
} // namespace application
} // namespace tst