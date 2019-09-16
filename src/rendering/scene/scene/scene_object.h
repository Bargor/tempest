// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <chrono>
#include <engine/resources/index_buffer.h>
#include <engine/resources/material.h>
#include <engine/resources/pipeline.h>
#include <engine/resources/uniform_buffer.h>
#include <engine/resources/vertex_buffer.h>

namespace tst {
namespace scene {

    class scene_object {
    public:
        struct state {
            const engine::resources::vertex_buffer* vertices;
            const engine::resources::index_buffer* indices;
            const engine::resources::pipeline* pipeline;
            const scene_object& object;
        };

    public:
        scene_object(engine::resources::vertex_buffer&& vertexBuffer,
                     engine::resources::index_buffer&& indexBuffer,
                     engine::resources::material&& material,
                     const engine::resources::pipeline* pipeline) noexcept;
        state& get_object_state() const;
        state update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) const;

    private:
        engine::resources::vertex_buffer m_vertices;
        engine::resources::index_buffer m_indices;
        engine::resources::material m_material;
        const engine::resources::pipeline* m_pipeline;

        state m_objectState;
    };

} // namespace scene
} // namespace tst
