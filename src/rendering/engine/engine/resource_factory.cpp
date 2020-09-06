// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "engine/device.h"

namespace tst {
namespace engine {

    resource_factory::resource_factory(device& device, application::data_loader& dataLoader)
        : api::resource_factory(device.create_resource_factory(dataLoader)) {
    }

    resource_factory::~resource_factory() {
    }

    resources::index_buffer resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
        return resources::index_buffer(create_buffer_creation_info(), std::move(indices));
    }

    resources::index_buffer resource_factory::create_index_buffer(std::vector<std::uint32_t>&& indices) {
        return resources::index_buffer(create_buffer_creation_info(), std::move(indices));
    }

    std::size_t resource_factory::create_pipeline(const std::string& techniqueName,
                                                  std::string_view pipelineName,
                                                  const std::string& shadersName,
                                                  const resources::vertex_buffer& vertexBuffer) {
        return super::create_pipeline(techniqueName, pipelineName, shadersName, vertexBuffer.to_super());
    }

    void resource_factory::create_technique(std::string name) {
        super::create_technique(std::move(name));
    }

    resources::vertex_buffer resource_factory::create_vertex_buffer(const engine::vertex_format& format,
                                                                    std::vector<vertex>&& vertices) {
        return resources::vertex_buffer(create_buffer_creation_info(), format, std::move(vertices));
    }

    resources::texture resource_factory::create_texture(const std::string& textureName) {
        return resources::texture(create_texture_creation_info(textureName));
    }

    api::buffer::creation_info resource_factory::create_buffer_creation_info() const noexcept {
        return super::create_buffer_creation_info();
    }

    api::uniform_buffer::creation_info resource_factory::create_uniform_creation_info(const std::string& shaderName,
                                                                                      bind_point bindPoint) const noexcept {
        return super::create_uniform_creation_info(shaderName, bindPoint);
    }

    api::texture::creation_info resource_factory::create_texture_creation_info(const std::string& textureName) const {
        return super::create_texture_creation_info(textureName);
    }

    api::material::creation_info resource_factory::create_material_creation_info(const std::string& shaderName,
                                                                                 const std::vector<std::string>& textureNames,
                                                                                 std::uint32_t staticStorageSize,
                                                                                 std::uint32_t dynamicStorageSize) const {
        return super::create_material_creation_info(shaderName, textureNames, staticStorageSize, dynamicStorageSize);
    }
} // namespace engine
} // namespace tst
