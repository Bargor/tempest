// This file is part of Tempest-scene project
// Author: Karol Kontny
#pragma once

#include "mesh.h"
#include "material.h"

namespace tst {
namespace engine {
    class model {
    public:
        model() = default;
        model(std::vector<mesh> meshes, std::vector<material> materials);

    public: 
        const mesh& get_mesh(std::uint32_t idx) const noexcept;
        const material& get_material(std::uint32_t idx) const noexcept;

        void add_material(material&& material);
    private:
        std::vector<mesh> m_meshes;
        std::vector<material> m_materials;
    };

    TST_INLINE const mesh& model::get_mesh(std::uint32_t idx) const noexcept {
        assert(idx < m_meshes.size());
        return m_meshes[idx];
    }

    TST_INLINE const material& model::get_material(std::uint32_t idx) const noexcept {
        assert(idx < m_materials.size());
        return m_materials[idx];
    }
} // namespace engine
} // namespace tst