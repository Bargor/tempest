// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <chrono>
#include <engine/resources/index_buffer.h>
#include <engine/resources/material.h>
#include <engine/resources/pipeline.h>
#include <engine/resources/texture.h>
#include <engine/resources/uniform_buffer.h>
#include <engine/resources/vertex_buffer.h>

namespace tst {
namespace scene {

    class scene_object {
    public:
        struct state {
            const engine::resources::vertex_buffer* vertices;
            const engine::resources::index_buffer* indices;
            const engine::resources::uniform_buffer& uniform;
            const engine::resources::texture& texture;
            const engine::resources::pipeline& pipeline;
            const scene_object& object;
        };

    public:
        scene_object(engine::resources::vertex_buffer&& vertexBuffer,
                     engine::resources::index_buffer&& indexBuffer,
                     engine::resources::material&& material,
                     engine::resources::uniform_buffer&& uniformBuffer,
                     engine::resources::texture&& texture,
                     const engine::resources::pipeline& pipeline) noexcept;
        scene_object(const scene_object&) = delete;
        scene_object(scene_object&& object) noexcept;

        state& get_object_state() const;
        state update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    private:
        engine::resources::vertex_buffer m_vertices;
        engine::resources::index_buffer m_indices;
        engine::resources::material m_material;
        engine::resources::uniform_buffer m_uniforms;
        engine::resources::texture m_texture;
        const engine::resources::pipeline& m_pipeline;

        state m_objectState;

        float m_time;
    };

} // namespace scene
} // namespace tst
