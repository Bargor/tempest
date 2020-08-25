// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/material.h>

namespace tst {
namespace engine {
    namespace opengl {
        class material : base::material {
        public:
            using super = base::material;

            material(const std::string& name);

        private:
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
