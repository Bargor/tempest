// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "engine/device.h"

namespace tst {
namespace engine {

    resource_factory::resource_factory(device& device, application::data_loader& dataLoader)
        : api::resource_factory(device, dataLoader) {
    }

    resource_factory::~resource_factory() {
    }

    resources::index_buffer resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
        return super::create_index_buffer<std::uint16_t>(std::move(indices));
    }

    resources::material resource_factory::create_material() {
        return resources::material{};
    }

    const resources::pipeline& resource_factory::create_pipeline(const std::string& techniqueName,
                                                                 const std::string& shadersName,
                                                                 const vertex_format& format) {
        return static_cast<const resources::pipeline&>(super::create_pipeline(
            techniqueName, shadersName, format.vertex_format::operator const tst::engine::vulkan::vertex_format&()));
    }

    void resource_factory::create_technique(std::string&& name, base::technique_settings&& settings) {
        super::create_technique(std::move(name), std::move(settings));
    }

    resources::vertex_buffer resource_factory::create_vertex_buffer(const engine::vertex_format& format,
                                                                    std::vector<vertex>&& vertices) {
        return super::create_vertex_buffer(format.vertex_format::operator const tst::engine::vulkan::vertex_format&(),
                                           std::move(vertices));
    }

    resources::uniform_buffer resource_factory::create_uniform_buffer(const std::string& shaderName,
                                                                      shader::shader_type type,
                                                                      std::uint32_t binding) {
        return super::create_uniform_buffer(shaderName, type, binding);
    }
} // namespace engine
} // namespace tst
