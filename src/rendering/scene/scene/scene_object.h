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

    struct uniform_buffer_object {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 asd;
    };

    class scene_object {
    public:
        struct state {
            const engine::model& model;
            const engine::resources::uniform_buffer& uniform;
            engine::pipeline_hash pipeline;
            const scene_object& object;
        };

    public:
        scene_object(std::string objectName,
                     engine::model&& model,
                     engine::resources::uniform_buffer&& uniformBuffer,
                     engine::pipeline_hash pipeline) noexcept;
        scene_object(const scene_object&) = delete;
        scene_object(scene_object&& object) noexcept;

        state& get_object_state() const;
        state update_object(std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    private:
        std::string m_name;
        engine::model m_model;
        engine::resources::uniform_buffer m_uniforms;
        engine::pipeline_hash m_pipeline;

        state m_objectState;

        float m_time;
    };

} // namespace scene
} // namespace tst
