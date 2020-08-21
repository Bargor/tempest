// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/vertex_buffer.h"
#include "material.h"

namespace tst {
namespace engine {

    class mesh {
        class surface {
            material* m_material;
        };
    public:
    private:
        std::vector<surface> m_surfaces;
    };

} // namespace engine
} // namespace tst