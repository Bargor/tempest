// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
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

    class resource_factory final : api::resource_factory {
        using super = api::resource_factory;

    public:
        resource_factory(device& device);
        ~resource_factory();

    public:
        resources::index_buffer create_index_buffer(std::vector<std::uint16_t>&& indices);
        resources::pipeline create_pipeline(base::pipeline&& pipeline,
                                            const std::string& techniqueName,
                                            const std::string& shadersName,
                                            const vertex_format& format);
        resources::vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
        resources::uniform_buffer create_uniform_buffer();

    private:
    };

    static_assert(!std::is_polymorphic_v<resource_factory>);
    static_assert(sizeof(resource_factory) == sizeof(api::resource_factory));
} // namespace engine
} // namespace tst
