// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "texture.h"

namespace tst {
namespace engine {
    namespace resources {
        texture::texture(const creation_info& info) : api::texture(info) {
        }

        texture::~texture() {
        }

        texture::texture(texture&& other) noexcept : api::texture(std::move(other)) {
        }

        void texture::bind_texture(const std::string& shaderName, bind_point set, std::uint32_t binding) {
            super::bind_texture(shaderName, set, binding);
        }
    } // namespace resources
} // namespace engine
} // namespace tst