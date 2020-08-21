// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/texture.h"
#include "resources/uniform_buffer.h"

#include <engine-impl/api.h>

namespace tst {
namespace engine {

    class material {
    public:
        material(const std::string& name, resources::uniform_buffer&& staticUniforms, resources::uniform_buffer&& dynamicUniforms);

    private:
        std::string m_name;
        resources::uniform_buffer m_staticUniformBuffer;
        resources::uniform_buffer m_dynamicUniformBuffer;
        std::vector<resources::texture> m_textures;
    };
} // namespace engine
} // namespace tst
