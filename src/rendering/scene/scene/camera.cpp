// This file is part of Tempest project
// Author: Karol Kontny

#include "camera.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <util/variant.h>

namespace tst {
namespace scene {

    camera::camera(application::event_processor<application::app_event>& eventProcessor,
                   const glm::vec3& position,
                   const glm::vec4& lookAt, const glm::vec3& up)
        : m_eventProcessor(eventProcessor), m_position(glm::vec4(position, 0.0f)), m_orientation(glm::quatLookAt(glm::normalize(lookAt - m_position).xyz(), up)) {
        auto key_callback = [](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::keyboard>(args));
        };

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::keyboard>(),
            this,
            std::move(key_callback));
    }
} // namespace scene
} // namespace tst