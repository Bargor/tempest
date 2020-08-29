// This file is part of Tempest-scene project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/vertex_buffer.h"

#include <optional>

namespace tst {
namespace engine {

    class material;

    class mesh {
    public:
        mesh(resources::vertex_buffer vertices);
        mesh(resources::vertex_buffer vertices, const material* material);
        mesh(resources::vertex_buffer vertices, resources::index_buffer indices, const material* material);

        mesh(mesh&& other) noexcept;

    private:
        resources::vertex_buffer m_vertices;
        std::optional<resources::index_buffer> m_indices;
        const material* m_material;
    };

} // namespace engine
} // namespace tst