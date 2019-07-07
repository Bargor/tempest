// This file is part of Tempest-engine-GL project
// Author: Karol Kontny
#pragma once

#include "vertex_format.h"

namespace tst {
namespace engine {
	namespace resources {

        class vertex_buffer {
        public:
            vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices);

        private:
        };
	}
}
}