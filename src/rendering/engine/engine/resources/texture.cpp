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
    }
}
}