// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "material.h"

namespace tst {
namespace engine {
    namespace vulkan {

        material::material(std::string&& name, const std::string& shaderName, const creation_info& info)
            : base::material(std::move(name))
            , m_textures()
            , m_staticUniformBuffer(info.staticUniformInfo ?
                                        uniform_buffer(info.staticUniformInfo.value(), 0, info.staticStorageSize) :
                                        std::optional<uniform_buffer>())
            , m_dynamicUniformBuffer(info.dynamicUniformInfo ?
                                         uniform_buffer(info.dynamicUniformInfo.value(), 0, info.dynamicStorageSize) :
                                         std::optional<uniform_buffer>())
            , m_staticDescriptorSets(info.staticDescriptorSets)
            , m_resourceIndex(info.resourceIndex) {
            std::uint32_t binding = info.staticStorageSize == 0 ? 0 : 1;
            for (const auto& textureInfo : info.textureInfos) {
                m_textures.emplace_back(textureInfo);
                m_textures.rbegin()->bind_texture(shaderName, base::resource_bind_point::material_static, binding++);
            }
        }

        material::material(std::string&& name,
                           std::vector<texture>&& textures,
                           const descriptor_set& staticDescriptorSets,
                           const std::uint32_t& resourceIndex)
            : base::material(std::move(name))
            , m_textures(std::move(textures))
            , m_staticUniformBuffer(std::nullopt)
            , m_dynamicUniformBuffer(std::nullopt)
            , m_staticDescriptorSets(staticDescriptorSets)
            , m_resourceIndex(resourceIndex) {
        }

        material::material(std::string&& name,
                           uniform_buffer&& staticUniforms,
                           uniform_buffer&& dynamicUniforms,
                           std::vector<texture>&& textures,
                           const descriptor_set& staticDescriptorSets,
                           const std::uint32_t& resourceIndex)
            : base::material(std::move(name))
            , m_textures(std::move(textures))
            , m_staticUniformBuffer(std::move(staticUniforms))
            , m_dynamicUniformBuffer(std::move(dynamicUniforms))
            , m_staticDescriptorSets(staticDescriptorSets)
            , m_resourceIndex(resourceIndex) {
        }

        material::material(material&& other) noexcept
            : base::material(std::move(other))
            , m_textures(std::move(other.m_textures))
            , m_staticUniformBuffer(std::move(other.m_staticUniformBuffer))
            , m_dynamicUniformBuffer(std::move(other.m_dynamicUniformBuffer))
            , m_staticDescriptorSets(other.m_staticDescriptorSets)
            , m_resourceIndex(other.m_resourceIndex) {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
