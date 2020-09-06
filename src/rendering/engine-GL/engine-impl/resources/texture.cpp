// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "texture.h"

namespace tst {
namespace engine {
    namespace opengl {

        texture::texture(const creation_info&) {
        }

        texture ::~texture() {
        }

        texture::texture(texture&&) noexcept {
        }

        void texture::bind_texture(const std::string&, base::resource_bind_point, std::uint32_t) {
        }

    } // namespace opengl
} // namespace engine
} // namespace tst