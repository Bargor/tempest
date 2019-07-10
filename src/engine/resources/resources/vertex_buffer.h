// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include "vertex_format.h"

#include <engine/api.h>
#include <engine/resources/vertex_buffer.h>

namespace tst {
namespace engine {
	namespace resources {

        class vertex_buffer {
        public:
            vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);
            ~vertex_buffer();

        private:
            api::vertex_buffer m_nativeVertexBuffer;
        };
	}
}
}