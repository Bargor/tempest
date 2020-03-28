// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/material.h"
#include "resources/pipeline.h"
#include "resources/shader.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/vertex_format.h"

#include <engine-impl/api.h>
#include <engine-impl/resource_factory.h>

namespace tst {
namespace engine {

    class device;

    class resource_factory final : private api::resource_factory {
        using super = api::resource_factory;

    public:
        resource_factory(device& device, application::data_loader& dataLoader);
        ~resource_factory();

    public:
        resources::index_buffer create_index_buffer(std::vector<std::uint16_t>&& indices);
        resources::material create_material();
        const resources::pipeline& create_pipeline(const std::string& techniqueName,
                                                   const std::string& pipelineName,
                                                   const std::string& shadersName,
                                                   const vertex_format& format);
        void create_technique(std::string&& name);
        resources::vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
        resources::uniform_buffer create_uniform_buffer(const std::string& shaderName,
                                                        api::shader_type type,
                                                        std::uint32_t binding);

    private:
    };

    static_assert(!std::is_polymorphic_v<resource_factory>);
    static_assert(sizeof(resource_factory) == sizeof(api::resource_factory));
} // namespace engine
} // namespace tst
