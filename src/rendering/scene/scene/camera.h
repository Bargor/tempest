// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <chrono>
#include <engine/resources/uniform_buffer.h>
#include <glm.h>
#include <string>

namespace tst {
namespace application {
    template<typename Event>
    class event_processor;

    struct app_event;
} // namespace application
namespace scene {
    class camera {
        struct input_delta {
            struct keyboard {
                bool moveForward = false;
                bool moveBackward = false;
                bool moveLeft = false;
                bool moveRight = false;
            };
            struct mouse {
                std::uint32_t xPos;
                std::uint32_t yPos;
            };

            keyboard keyboardInput;
            mouse mouseInput;
        };

    public:
        struct uniforms {
            glm::mat4 view;
            glm::mat4 perspective;
            glm::mat4 viewPerspective;
            glm::mat4 position;
        };

    public:
        camera(const std::string& cameraName,
               application::event_processor<application::app_event>& eventProcessor,
               engine::resources::uniform_buffer&& buffer,
               const glm::vec3& position,
               const glm::vec3& lookAt,
               const glm::vec3& up,
               const float fov,
               const float aspect);

        camera(const camera&) = delete;

        std::string_view get_name() const noexcept;
        const engine::resources::uniform_buffer& get_uniforms() const;

        void update(std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    private: 
        glm::vec4 caclulate_position(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) const;
        glm::quat calculate_orientation(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) const;

    private:
        std::string m_name;
        application::event_processor<application::app_event>& m_eventProcessor;
        engine::resources::uniform_buffer m_buffer;
        glm::vec4 m_position;
        glm::quat m_orientation;
        glm::mat4 m_perspective;
        float m_moveSensitivity;
        float m_rotateSensitivity;
        input_delta m_input;
    };

    TST_INLINE std::string_view camera::get_name() const noexcept {
        return m_name;
    }

    TST_INLINE const engine::resources::uniform_buffer& camera::get_uniforms() const {
        return m_buffer;
    }
} // namespace scene
} // namespace tst