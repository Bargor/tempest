// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

namespace tst {
namespace engine {
    namespace opengl {
        resource_factory::resource_factory(device& device, application::data_loader& dataLoader)
            : m_device(device), m_dataLoader(dataLoader) {
        }

        resource_factory::~resource_factory() {
        }

        vertex_buffer resource_factory::create_vertex_buffer(const vertex_format& format,
                                                             std::vector<vertex>&& vertices) {
            return m_device.create_vertex_buffer(format, std::move(vertices));
        }

        uniform_buffer resource_factory::create_uniform_buffer() {
            return m_device.create_uniform_buffer();
        }
    }
}
}