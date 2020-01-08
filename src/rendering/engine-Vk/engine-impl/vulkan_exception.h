// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <string>
#include <stdexcept>

namespace tst {
namespace engine {
    namespace vulkan {

    class vulkan_exception : public std::runtime_error {
        using super = std::runtime_error;
    public:

        explicit vulkan_exception(const std::string& message) : super(("Vulkan exception: " + message).c_str()) {
        }

        explicit vulkan_exception(const char* message) : super(message) {
        }
    };

} // vulkan
} // namespace engine
} // namespace tst