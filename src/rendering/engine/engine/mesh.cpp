// This file is part of Tempest-scene project
// Author: Karol Kontny

#include "mesh.h"

namespace tst {
namespace engine {

    mesh::mesh(resources::vertex_buffer vertices)
        : m_vertices(std::move(vertices)), m_indices(std::nullopt), m_material(nullptr) {
    }

    mesh::mesh(resources::vertex_buffer vertices, const material* material)
        : m_vertices(std::move(vertices)), m_indices(std::nullopt), m_material(material) {
    }

    mesh::mesh(resources::vertex_buffer vertices, resources::index_buffer indices, const material* material)
        : m_vertices(std::move(vertices)), m_indices(std::move(indices)), m_material(material) {
    }

    mesh::mesh(mesh&& other) noexcept
        : m_vertices(std::move(other.m_vertices)), m_indices(std::move(other.m_indices)), m_material(other.m_material) {
    }
} // namespace engine
} // namespace tst