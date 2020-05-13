// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <glm.h>

#include <engine/resources/uniform_buffer.h>

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

            keyboard keyboardInput;
        };
    public:
        camera(application::event_processor<application::app_event>& eventProcessor,
               engine::resources::uniform_buffer&& buffer,
               const glm::vec3& position,
               const glm::vec3& lookAt,
               const glm::vec3& up);

        void update(std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    private:
        application::event_processor<application::app_event>& m_eventProcessor;
        engine::resources::uniform_buffer m_buffer;
        glm::vec4 m_position;
        glm::quat m_orientation;
        input_delta m_input;
    };
} // namespace scene
} // namespace tst