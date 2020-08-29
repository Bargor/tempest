// This file is part of Tempest-scene project
// Author: Karol Kontny

#include "model.h"

namespace tst {
namespace engine {
    model::model(std::vector<mesh> meshes, std::vector<material> materials)
        : m_meshes(std::move(meshes)), m_materials(std::move(materials)) {
    }
} // namespace engine
} // namespace tst