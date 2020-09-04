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

        ~mesh();

    public:
        const resources::vertex_buffer& get_vertices() const noexcept;
        const resources::index_buffer* get_indices() const noexcept;
        const material* get_material() const noexcept;

        void set_material(const material& material);

    private:
        resources::vertex_buffer m_vertices;
        std::optional<resources::index_buffer> m_indices;
        const material* m_material;
    };

    TST_INLINE const resources::vertex_buffer& mesh::get_vertices() const noexcept {
        return m_vertices;
    }

    TST_INLINE const resources::index_buffer* mesh::get_indices() const noexcept {
        return &*m_indices;
    }

    TST_INLINE const material* mesh::get_material() const noexcept {
        return m_material;
    }

    TST_INLINE void mesh::set_material(const material& material) {
        m_material = &material;
    }

} // namespace engine
} // namespace tst