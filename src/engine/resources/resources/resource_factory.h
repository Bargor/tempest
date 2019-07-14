// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "index_buffer.h"
#include "uniform_buffer.h"
#include "vertex_buffer.h"
#include "vertex_format.h"

#include <engine/api.h>
#include <engine/resources/index_buffer.h>

namespace tst {
namespace engine {

	namespace api{
        class device;
	}

    namespace resources {

        class resource_factory {
        public:
            resource_factory(api::device& device);
            ~resource_factory();

        public:
            index_buffer create_index_buffer(std::vector<std::uint16_t>&& indices);
            vertex_buffer create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
            uniform_buffer create_uniform_buffer();

        private:
            api::device& m_nativeDevice;
            vk::CommandPool m_commandPool;
        };

    } // namespace resources
} // namespace engine
} // namespace tst