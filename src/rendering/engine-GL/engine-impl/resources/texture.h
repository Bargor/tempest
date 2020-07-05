// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <application/image_data.h>
#include <engine-base/resource_bind_point.h>

namespace tst {
namespace engine {
    namespace opengl {

        class texture {
        public:
            texture(const application::image_data& imageData);
            ~texture();

            texture(texture&& other) noexcept;

        public:
            void bind_texture(const std::string& shaderName, base::resource_bind_point type, std::uint32_t binding);

        private:
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
