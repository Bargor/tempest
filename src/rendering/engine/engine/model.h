// This file is part of Tempest-scene project
// Author: Karol Kontny
#pragma once

#include "mesh.h"
#include "material.h"

namespace tst {
namespace engine {
    class model {
    public:
        model() = default;
        model(std::vector<mesh> meshes, std::vector<material> materials);
    private:
        std::vector<mesh> m_meshes;
        std::vector<material> m_materials;
    };
} // namespace engine
} // namespace tst