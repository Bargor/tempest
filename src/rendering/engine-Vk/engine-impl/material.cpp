// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "material.h"

namespace tst {
namespace engine {
    namespace vulkan {

        material::material(const std::string& name,
                           std::vector<texture>&& textures,
                           const descriptor_set& staticDescriptorSets,
                           const std::uint32_t& resourceIndex)
            : base::material(name)
            , m_textures(std::move(textures))
            , m_staticUniformBuffer(std::nullopt)
            , m_dynamicUniformBuffer(std::nullopt)
            , m_staticDescriptorSets(staticDescriptorSets)
            , m_resourceIndex(resourceIndex) {
        }

        material::material(const std::string& name,
                           uniform_buffer&& staticUniforms,
                           uniform_buffer&& dynamicUniforms,
                           std::vector<texture>&& textures,
                           const descriptor_set& staticDescriptorSets,
                           const std::uint32_t& resourceIndex)
            : base::material(name)
            , m_textures(std::move(textures))
            , m_staticUniformBuffer(std::move(staticUniforms))
            , m_dynamicUniformBuffer(std::move(dynamicUniforms))
            , m_staticDescriptorSets(staticDescriptorSets)
            , m_resourceIndex(resourceIndex) {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
