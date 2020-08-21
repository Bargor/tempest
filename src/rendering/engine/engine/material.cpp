// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "material.h"

#include "resource_factory.h"

namespace tst {
namespace engine {

    material::material(const std::string& name,
                       resources::uniform_buffer&& staticUniforms,
                       resources::uniform_buffer&& dynamicUniforms)
        : m_name(name)
        , m_staticUniformBuffer(std::move(staticUniforms))
        , m_dynamicUniformBuffer(std::move(dynamicUniforms)) {
    }

} // namespace engine
} // namespace tst
