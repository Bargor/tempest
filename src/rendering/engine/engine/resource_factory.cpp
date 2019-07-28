// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "engine/device.h"

namespace tst {
namespace engine {

    resource_factory::resource_factory(device& device) : api::resource_factory(device) {
    }

    resource_factory::~resource_factory() {
    }

    resources::index_buffer resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
        return super::create_index_buffer<std::uint16_t>(std::move(indices));
    }

    resources::vertex_buffer resource_factory::create_vertex_buffer(const engine::vertex_format& format,
                                                         std::vector<vertex>&& vertices) {
        return super::create_vertex_buffer(format, std::move(vertices));
    }

    resources::uniform_buffer resource_factory::create_uniform_buffer() {
        return super::create_uniform_buffer();
    }
} // namespace engine
} // namespace tst