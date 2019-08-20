// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <memory>

namespace tst {
namespace engine {

    namespace vulkan {
        class device;
        class draw_command;
        class gpu_info;
        class rendering_engine;
        class vertex_format;

        template<typename T>
        using ptr = std::unique_ptr<T>;
    } // namespace vulkan

    namespace api = vulkan;
} // namespace engine
} // namespace tst
