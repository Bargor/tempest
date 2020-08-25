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
        return super::create_index_buffer<std::uint16_t>(std::move(indices));
    }

    const resources::pipeline& resource_factory::create_pipeline(const std::string& techniqueName,
                                                                 const std::string& pipelineName,
                                                                 const std::string& shadersName,
                                                                 const vertex_format& format) {
        return (const resources::pipeline&)(super::create_pipeline(
            techniqueName, pipelineName, shadersName, format.to_super()));
    }

    void resource_factory::create_technique(const std::string& name) {
        super::create_technique(name);
    }

    resources::vertex_buffer resource_factory::create_vertex_buffer(const engine::vertex_format& format,
                                                                    std::vector<vertex>&& vertices) {
        return super::create_vertex_buffer(format.to_super(), std::move(vertices));
    }

    resources::texture resource_factory::create_texture(const std::string& textureName) {
        return super::create_texture(textureName);
    }
} // namespace engine
} // namespace tst
