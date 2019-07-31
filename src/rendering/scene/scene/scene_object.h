// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <chrono>
#include <engine/resources/index_buffer.h>
#include <engine/resources/uniform_buffer.h>
#include <engine/resources/vertex_buffer.h>

namespace tst {
namespace scene {

    class scene_object {
    public:
        using vertex_buffers = std::vector<engine::resources::vertex_buffer>;
        using index_buffers = std::vector<engine::resources::index_buffer>;

        struct state {
            engine::api::uniform_buffer_object transformation;
            const engine::resources::vertex_buffer* vertices;
            const engine::resources::index_buffer* indices;
            const scene_object& object;
        };

    public:
        scene_object(engine::resources::vertex_buffer&& vertexBuffer, engine::resources::index_buffer&& indexBuffer) noexcept;
        state& get_object_state() const;
        state update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) const;

    private:
        engine::resources::vertex_buffer m_vertices;
        engine::resources::index_buffer m_indices;
        state m_objectState;
    };

} // namespace scene
} // namespace tst