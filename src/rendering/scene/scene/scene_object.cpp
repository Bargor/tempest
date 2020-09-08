// This file is part of Tempest project
// Author: Karol Kontny

#include "scene_object.h"

#include "camera.h"

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
        , m_objectStaticData({m_model, m_uniforms, m_pipeline, *this})
        , m_dynamicData(render_data{glm::mat4(1.0f)})
        , m_prevDynamicData(render_data{glm::mat4(1.0f)})
        , m_time(0.0f) {
    }

    scene_object::scene_object(scene_object&& other) noexcept
        : m_name(std::move(other.m_name))
        , m_model(std::move(other.m_model))
        , m_uniforms(std::move(other.m_uniforms))
        , m_pipeline(other.m_pipeline)
        , m_objectStaticData({m_model, m_uniforms, m_pipeline, *this})
        , m_dynamicData(other.m_dynamicData)
        , m_prevDynamicData(other.m_prevDynamicData)
        , m_time(0.0f) {
    }

    scene_object::static_data& scene_object::get_static_data() {
        return m_objectStaticData;
    }

    void scene_object::update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) {
        float time = std::chrono::duration<float, std::chrono::seconds::period>(elapsedTime).count();
        m_time += time;
    }

    scene_object::render_data scene_object::prepare_render_data(const camera& camera) {
        m_dynamicData = render_data{glm::mat4(1.0f)};

        uniform_buffer_object ubo{{}, camera.getPerspectiveMatrix() * camera.getViewMatrix() * m_dynamicData.model };

        m_uniforms.update_buffer(ubo);

        return m_dynamicData;
    }

} // namespace scene
} // namespace tst
