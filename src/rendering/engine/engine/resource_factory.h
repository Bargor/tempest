// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
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
            resources::vertex_buffer create_vertex_buffer(const engine::vertex_format& format,
                                                          std::vector<vertex>&& vertices);
            resources::uniform_buffer create_uniform_buffer();

        private:
        };
} // namespace engine
} // namespace tst