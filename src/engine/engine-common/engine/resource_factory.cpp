// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "engine/device.h"

namespace tst {
namespace engine {

    resource_factory::resource_factory(device& device) : m_factoryImpl(device.m_deviceImpl) {
    }

    resource_factory::~resource_factory() {
    }

    resources::index_buffer resource_factory::create_index_buffer(std::vector<std::uint16_t>&& indices) {
        return m_factoryImpl.create_index_buffer<std::uint16_t>(std::move(indices));
    }

    resources::vertex_buffer resource_factory::create_vertex_buffer(const engine::vertex_format& format,
                                                         std::vector<vertex>&& vertices) {
        return m_factoryImpl.create_vertex_buffer(format, std::move(vertices));
    }

    resources::uniform_buffer resource_factory::create_uniform_buffer() {
        return m_factoryImpl.create_uniform_buffer();
    }
} // namespace engine
} // namespace tst