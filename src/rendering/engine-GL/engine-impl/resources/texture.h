// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <application/image_data.h>

namespace tst {
namespace engine {
    namespace opengl {

        class texture {
        public:
            texture(const application::image_data& imageData);
            ~texture();

            texture(texture&& other) noexcept;

        private:
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
