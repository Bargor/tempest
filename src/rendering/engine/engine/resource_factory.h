// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "material.h"
#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/texture.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/vertex_format.h"

#include <engine-impl/api.h>
#include <engine-impl/resource_factory.h>

namespace tst {
namespace engine {

    class device;

    namespace resources {
        class pipeline;
    }

    using bind_point = base::resource_bind_point;

    class resource_factory final : private api::resource_factory {
        using super = api::resource_factory;

    public:
        resource_factory(device& device, application::data_loader& dataLoader);
        ~resource_factory();

    public:
        template<MaterialType T>
        material create_material(std::string materialName,
                                 const std::string& shaderName,
                                 const std::vector<std::string>& textureNames);

        resources::index_buffer create_index_buffer(std::vector<std::uint16_t>&& indices);
        resources::index_buffer create_index_buffer(std::vector<std::uint32_t>&& indices);
        pipeline_hash create_pipeline(const std::string& techniqueName,
                                      std::string_view pipelineName,
                                      const std::string& shadersName,
                                      const resources::vertex_buffer& vertexBuffer);
        void create_technique(std::string name);
        resources::vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
        resources::texture create_texture(const std::string& textureName);
        template<typename StorageType>
        resources::uniform_buffer create_uniform_buffer(const std::string& shaderName,
                                                        bind_point bindPoint,
                                                        std::uint32_t binding);

    private:
        api::buffer_construction_info create_buffer_construction_info() const noexcept;
    };

    template<typename StorageType>
    resources::uniform_buffer resource_factory::create_uniform_buffer(const std::string& shaderName,
                                                                      bind_point bindPoint,
                                                                      std::uint32_t binding) {
        return super::create_uniform_buffer(shaderName, bindPoint, binding, sizeof(StorageType));
    }

    template<MaterialType T>
    material resource_factory::create_material(std::string materialName,
                                               const std::string& shaderName,
                                               const std::vector<std::string>& textureNames) {
        return super::create_material(
            std::move(materialName),
            shaderName,
            textureNames,
            std::is_null_pointer_v<typename T::StaticStorageType> ? 0 : sizeof(typename T::StaticStorageType),
            std::is_null_pointer_v<typename T::DynamicStorageType> ? 0 : sizeof(typename T::DynamicStorageType));
    }

    static_assert(!std::is_polymorphic_v<resource_factory>);
    static_assert(sizeof(resource_factory) == sizeof(api::resource_factory));
} // namespace engine
} // namespace tst
