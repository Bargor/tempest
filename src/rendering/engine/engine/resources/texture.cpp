// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "texture.h"

namespace tst {
namespace engine {
    namespace resources {
        texture::texture(api::texture&& textureImpl) : api::texture(std::move(textureImpl)) {
        }

        texture::~texture() {
        }

        texture::texture(texture&& other) noexcept : api::texture(std::move(other)) {
        }

        void texture::bind_texture(const std::string shaderName, api::shader_type type, std::uint32_t binding) {
            super::bind_texture(shaderName, type, binding);
        }
    }
}
}