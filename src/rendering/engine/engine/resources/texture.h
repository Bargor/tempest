// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"

#include <engine-impl/api.h>
#include <engine-impl/resources/texture.h>

namespace tst {
namespace engine {
    namespace resources {

        class texture : private api::texture {
            using super = api::texture;

        public:
            texture(api::texture&& textureImpl);
            ~texture();

            texture(texture&& other) noexcept;

        public:
            void bind_texture(const std::string shaderName, api::shader_type type, std::uint32_t binding);

        public:
            const api::texture& to_super() const noexcept {
                return *this;
            }

        private:
        };

        static_assert(!std::is_polymorphic_v<texture>);
        static_assert(sizeof(texture) == sizeof(api::texture));
    } // namespace resources
} // namespace engine
} // namespace tst