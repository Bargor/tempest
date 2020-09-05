// This file is part of Tempest project
// Author: Karol Kontny

#include "scene_object.h"

#include <glm.h>

namespace tst {
namespace scene {

    scene_object::scene_object(std::string objectName,
                               engine::model&& model,
                               engine::resources::uniform_buffer&& uniformBuffer,
                               engine::pipeline_hash pipeline) noexcept
        : m_name(std::move(objectName))
        , m_model(std::move(model))
        , m_uniforms(std::move(uniformBuffer))
        , m_pipeline(pipeline)
        , m_objectState({m_model, m_uniforms, m_pipeline, *this})
        , m_time(0.0f) {
    }

    scene_object::scene_object(scene_object&& object) noexcept
        : m_name(std::move(object.m_name))
        , m_model(std::move(object.m_model))
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

        state newState{m_model, m_uniforms, m_pipeline, *this};

        return newState;
    }

} // namespace scene
} // namespace tst
