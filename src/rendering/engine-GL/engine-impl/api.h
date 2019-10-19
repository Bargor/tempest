// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include <memory>

namespace tst {
namespace engine {

    namespace opengl {
        class device;
        class gpu_info;
        class rendering_engine;
        class vertex_format;

        template<typename T>
        using ptr = std::unique_ptr<T>;
    } // namespace opengl

    namespace api = opengl;
} // namespace engine
} // namespace tst
