// This file is part of Tempest-application project
// Author: Karol Kontny
#pragma once

#include <stdexcept>

namespace tst {
namespace application {

    class data_exception : public std::runtime_error {
        using super = std::runtime_error;
    public:

        explicit data_exception(const std::string& message) : super(message.c_str()) {}

        explicit data_exception(const char* message) : super(message) {}
    };
} // namespace application
} // namespace tst