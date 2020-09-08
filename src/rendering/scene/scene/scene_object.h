// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <chrono>
#include <engine/material.h>
#include <engine/model.h>
#include <engine/resources/pipeline.h>
#include <engine/resources/uniform_buffer.h>

namespace tst {
namespace scene {

    class camera;

    struct uniform_buffer_object : engine::resources::uniform_storage {
        glm::mat4 MVP;
    };

    static_assert(sizeof(uniform_buffer_object) == 256);

    class scene_object {
    public:
        struct static_data {
            const engine::model& model;
            const engine::resources::uniform_buffer& uniform;
            engine::pipeline_hash pipeline;
            scene_object& object;
        };
        struct render_data {
            glm::mat4 model;
        };

    public:
        scene_object(std::string objectName,
                     engine::model&& model,
                     engine::resources::uniform_buffer&& uniformBuffer,
                     engine::pipeline_hash pipeline) noexcept;
        scene_object(const scene_object&) = delete;
        scene_object(scene_object&& object) noexcept;

        static_data& get_static_data();
        void update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime);
        render_data prepare_render_data(const camera& camera);

    private:
        std::string m_name;
        engine::model m_model;
        engine::resources::uniform_buffer m_uniforms;
        engine::pipeline_hash m_pipeline;

        static_data m_objectStaticData;
        render_data m_dynamicData;
        render_data m_prevDynamicData;

        float m_time;
    };

} // namespace scene
} // namespace tst
