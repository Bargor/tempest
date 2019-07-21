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
            const vertex_buffers* vertices;
            const index_buffers* indices;
        };

    public:
        scene_object(vertex_buffers&& vertexBuffers, index_buffers&& indexBuffers) noexcept;
        state& get_object_state() const;
        state update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) const;

    private:
        vertex_buffers m_vertices;
        index_buffers m_indices;
        state m_objectState;
    };

} // namespace scene
} // namespace tst