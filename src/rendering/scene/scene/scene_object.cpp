// This file is part of Tempest project
// Author: Karol Kontny

#include "scene_object.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace tst {
namespace scene {

    scene_object::scene_object(engine::resources::vertex_buffer&& vertexBuffer,
                               engine::resources::index_buffer&& indexBuffer,
                               engine::resources::material& material) noexcept
        : m_vertices(std::move(vertexBuffer))
        , m_indices(std::move(indexBuffer))
        , m_objectState(state{engine::api::uniform_buffer_object{glm::mat4(), glm::mat4(), glm::mat4()},
                              &vertexBuffer,
                              &indexBuffer,
                              *this}) {
    }

    scene_object::state scene_object::update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) const {
        float time = std::chrono::duration<float, std::chrono::seconds::period>(elapsedTime).count();
        engine::api::uniform_buffer_object ubo;

        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        state newState{ubo, &m_vertices, &m_indices, *this};

        newState.transformation = ubo;

        return newState;
    }

} // namespace scene
} // namespace tst
