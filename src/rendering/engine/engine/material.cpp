// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "material.h"

#include "resource_factory.h"

namespace tst {
namespace engine {

    material::material(api::material&& materialImpl) : api::material(std::move(materialImpl)) {
    }

    material::material(material&& other) noexcept : api::material(std::move(other)) {
    }

} // namespace engine
} // namespace tst
