// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "material.h"

#include "resource_factory.h"

namespace tst {
namespace engine {

    material::material(std::string&& name, const std::string& shaderName, const creation_info& info)
        : api::material(std::move(name), shaderName, info) {
    }

    material::material(material&& other) noexcept : api::material(std::move(other)) {
    }

} // namespace engine
} // namespace tst
