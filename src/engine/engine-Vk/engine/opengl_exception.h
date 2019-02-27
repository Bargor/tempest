// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include <string>
#include <stdexcept>

namespace tst {
namespace engine {
    namespace backend {

    class api_exception : public std::runtime_error {
    public:
        using super = std::runtime_error;

        explicit api_exception(const std::string& message) : super(message.c_str()) {
        }

        explicit api_exception(const char* message) : super(message) {
        }
    };
} // backend
} // namespace engine
} // namespace tst