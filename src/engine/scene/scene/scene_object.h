// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <resources/vertex_buffer.h>
#include <resources/index_buffer.h>

namespace tst {
namespace scene {

	class scene_object {
    public:
    private:
        std::vector<engine::resources::vertex_buffer> m_vertices;
        std::vector<engine::resources::index_buffer> m_indices;
	};

}
} // namespace tst