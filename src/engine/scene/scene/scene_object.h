// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <chrono>
#include <resources/index_buffer.h>
#include <resources/uniform_buffer.h>
#include <resources/vertex_buffer.h>

namespace tst {
namespace scene {

    class scene_object {
    public:
        struct state {
            engine::api::uniform_buffer_object transformation;
            const std::vector<engine::resources::vertex_buffer>* vertices;
            const std::vector<engine::resources::index_buffer>* indices;
        };

    public:
        state& get_object_state() const;
        state update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) const;

    private:
        std::vector<engine::resources::vertex_buffer> m_vertices;
        std::vector<engine::resources::index_buffer> m_indices;
        state m_objectState;
    };

} // namespace scene
} // namespace tst