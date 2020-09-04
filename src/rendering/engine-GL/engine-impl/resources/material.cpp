// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "material.h"

namespace tst {
namespace engine {
    namespace opengl {
        material::material(
            std::string&& name, const std::string&, const std::vector<std::string>&, std::uint32_t, std::uint32_t)
            : base::material(std::move(name)) {
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
