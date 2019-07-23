// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <vector>

namespace tst {
namespace engine {
    namespace opengl {
		class device {
            public:
                device();
                ~device();

            template<typename IndexType>
            index_buffer<IndexType> create_index_buffer(std::vector<IndexType>&& indices) const;
            vertex_buffer create_vertex_buffer(const vertex_format& format,
                                               std::vector<vertex>&& vertices) const;
            uniform_buffer create_uniform_buffer() const;

            private:

		};

        template<typename IndexType>
        index_buffer<IndexType> device::create_index_buffer(std::vector<IndexType>&& indices) const {
            return index_buffer<std::uint16_t>(std::move(indices));
        }
	}
}
}