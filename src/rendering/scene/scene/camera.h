// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <glm.h>

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
               const glm::vec3& position,
               const glm::vec4& lookAt,
               const glm::vec3& up);

    private:
        application::event_processor<application::app_event>& m_eventProcessor;
        glm::vec4 m_position;
        glm::quat m_orientation;
        input_delta m_input;
    };
} // namespace scene
} // namespace tst