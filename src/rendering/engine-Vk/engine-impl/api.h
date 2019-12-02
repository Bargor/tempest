// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <memory>

template<typename T>
using ptr = std::unique_ptr<T>;

namespace tst {
namespace engine {

    namespace vulkan {
        class device;
        class draw_command;
        class gpu_info;
        class engine_frontend;
        class vertex_format;
    } // namespace vulkan

    namespace api = vulkan;
} // namespace engine
} // namespace tst
