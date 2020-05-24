// This file is part of Tempest project
// Author: Karol Kontny

#include "camera.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <device/keyboard.h>
#include <util/variant.h>

namespace tst {
namespace scene {

    camera::camera(const std::string& cameraName,
                   application::event_processor<application::app_event>& eventProcessor,
                   engine::resources::uniform_buffer&& buffer,
                   const glm::vec3& position,
                   const glm::vec3& lookAt,
                   const glm::vec3& up,
                   const float fov,
                   const float aspect)
        : m_name(cameraName)
        , m_eventProcessor(eventProcessor)
        , m_buffer(std::move(buffer))
        , m_position(glm::vec4(position, 0.0f))
        , m_orientation(glm::quatLookAt(glm::normalize(lookAt - m_position.xyz()), up))
        , m_perspective(glm::perspective(glm::radians(fov), aspect, 0.01f, 10.0f))
        , m_input() {
        auto key_callback = [&](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::keyboard>(args));
            input_delta::keyboard keyboardInput;
            if (std::get<application::app_event::keyboard>(args).key == static_cast<std::uint32_t>(device::keys::key_w)) {
                keyboardInput.moveForward = true;
            }
            if (std::get<application::app_event::keyboard>(args).key == static_cast<std::uint32_t>(device::keys::key_s)) {
                keyboardInput.moveBackward = true;
            }
            if (std::get<application::app_event::keyboard>(args).key == static_cast<std::uint32_t>(device::keys::key_a)) {
                keyboardInput.moveLeft = true;
            }
            if (std::get<application::app_event::keyboard>(args).key == static_cast<std::uint32_t>(device::keys::key_d)) {
                keyboardInput.moveRight = true;
            }
            m_input.keyboardInput = keyboardInput;
        };

        auto resize_callback = [&](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::framebuffer>(args));
            const auto size = std::get<application::app_event::framebuffer>(args).size;
            m_perspective =
                glm::perspective(glm::radians(fov), static_cast<float>(size.width) / size.height, 0.01f, 10.0f);
        };

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::keyboard>(),
            this,
            std::move(key_callback));

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::framebuffer>(),
            this,
            std::move(resize_callback));
    }

    void camera::update(std::chrono::duration<std::uint64_t, std::micro>) {
        m_buffer.update_buffer<uniforms>(
            {glm::toMat4(m_orientation), m_perspective, glm::toMat4(m_orientation) * m_perspective, glm::mat4()});
    }
} // namespace scene
} // namespace tst