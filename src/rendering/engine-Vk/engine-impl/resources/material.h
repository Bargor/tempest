// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "texture.h"
#include "uniform_buffer.h"

#include <engine-base/material.h>
#include <vector>

namespace tst {
namespace engine {
    namespace vulkan {
        class material : base::material {
            using super = base::material;

        public:
            material();

        private:
            std::vector<texture> m_textures;
            //uniform_buffer m_uniforms;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
