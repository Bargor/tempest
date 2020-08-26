// This file is part of Tempest project
// Author: Karol Kontny

#include "scene_object.h"

#include <glm.h>

namespace tst {
namespace scene {

    scene_object::scene_object(std::string objectName,
                               engine::resources::vertex_buffer&& vertexBuffer,
                               engine::resources::index_buffer&& indexBuffer,
                               engine::material&& material,
                               engine::resources::uniform_buffer&& uniformBuffer,
                               const engine::resources::pipeline& pipeline) noexcept
        : m_name(std::move(objectName))
        , m_vertices(std::move(vertexBuffer))
        , m_indices(std::move(indexBuffer))
        , m_material(std::move(material))
        , m_uniforms(std::move(uniformBuffer))
        , m_pipeline(pipeline)
        , m_objectState({&m_vertices, &m_indices, m_uniforms, m_pipeline, m_material, *this})
        , m_time(0.0f) {
    }

    scene_object::scene_object(scene_object&& object) noexcept
        : m_name(std::move(object.m_name))
        , m_vertices(std::move(object.m_vertices))
        , m_indices(std::move(object.m_indices))
        , m_material(std::move(object.m_material))
        , m_uniforms(std::move(object.m_uniforms))
        , m_pipeline(object.m_pipeline)
        , m_objectState(std::move(object.m_objectState))
        , m_time(0.0f) {
    }

    scene_object::state scene_object::update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) {
        float time = std::chrono::duration<float, std::chrono::seconds::period>(elapsedTime).count();
        m_time += time;
        uniform_buffer_object ubo;

        ubo.model = glm::mat4(1.0f); // glm::rotate(glm::mat4(1.0f), m_time * glm::radians(90.0f), glm::vec3(0.0f,
                                     // 0.0f, 1.0f));

        m_uniforms.update_buffer(ubo);

        state newState{&m_vertices, &m_indices, m_uniforms, m_pipeline, m_material, *this};

        return newState;
    }

} // namespace scene
} // namespace tst
