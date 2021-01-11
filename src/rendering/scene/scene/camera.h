// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <chrono>
#include <engine/resources/uniform_buffer.h>
#include <engine/resources/view.h>
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
                std::int32_t xPos = 0;
                std::int32_t yPos = 0;
            };

            keyboard keyboardInput;
            mouse mouseInput;
            bool newMousePos = true;
        };

    public:
        camera(std::string cameraName,
               application::event_processor<application::app_event>& eventProcessor,
               const glm::vec3& position,
               const glm::vec3& lookAt,
               const glm::vec3& up,
               float fov,
               float aspect,
               bool startSubscribed);

        camera(const camera&) = delete;
        camera(camera&& camera) noexcept;

        std::string_view get_name() const noexcept;

        void update(std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

        const engine::view& get_view() const noexcept;

    private:
        glm::vec4 caclulate_position_delta(float elapsedTime) const;
        glm::quat calculate_pitch(float elapsedTime) const;
        glm::quat calculate_yaw(float elapsedTime) const;

        void subscribe();
        void unsubscribe();

    private:
        std::string m_name;
        application::event_processor<application::app_event>& m_eventProcessor;
        engine::view m_view;
        float m_moveSensitivity;
        float m_rotateSensitivity;
        input_delta m_input;
    };

    TST_INLINE std::string_view camera::get_name() const noexcept {
        return m_name;
    }

    TST_INLINE const engine::view& camera::get_view() const noexcept {
        return m_view;
    }
} // namespace scene
} // namespace tst