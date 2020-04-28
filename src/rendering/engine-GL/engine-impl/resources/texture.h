// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"

#include <application/image_data.h>

namespace tst {
namespace engine {
    namespace opengl {

        class texture {
        public:
            texture(const application::image_data& imageData);
            ~texture();

            texture(texture&& other) noexcept;

        public:
            void bind_texture(const std::string& shaderName, shader_type type, std::uint32_t binding);

        private:
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
