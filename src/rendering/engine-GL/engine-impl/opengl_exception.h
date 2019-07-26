// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <string>
#include <stdexcept>

namespace tst {
namespace engine {
    namespace opengl {

    class opengl_exception : public std::runtime_error {
    public:
        using super = std::runtime_error;

        explicit opengl_exception(const std::string& message) : super(message.c_str()) {
        }

        explicit opengl_exception(const char* message) : super(message) {
        }
    };
} // backend
} // namespace engine
} // namespace tst