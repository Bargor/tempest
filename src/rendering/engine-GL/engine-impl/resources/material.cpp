// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "material.h"

namespace tst {
namespace engine {
    namespace opengl {
        material::material(std::string&& name, const std::string&, const creation_info&)
            : base::material(std::move(name)) {
        }
    } // namespace opengl
} // namespace engine
} // namespace tst
