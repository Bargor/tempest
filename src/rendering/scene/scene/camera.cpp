// This file is part of Tempest project
// Author: Karol Kontny

#include "camera.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <device/keyboard.h>
#include <fmt/printf.h>
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
        , m_direction(glm::normalize(lookAt - m_position.xyz()))
        , m_orientation(glm::quatLookAt(m_direction, up))
        , m_perspective(glm::perspective(glm::radians(fov), aspect, 0.01f, 100.0f))
        , m_moveSensitivity(3.0f)
        , m_rotateSensitivity(0.5f)
        , m_input() {
        m_perspective[1][1] *= -1;
        auto key_callback = [this](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::keyboard>(args));
            input_delta::keyboard keyboardInput;
            application::app_event::keyboard key_action = std::get<application::app_event::keyboard>(args);
            if (key_action.action == device::key_action::press || key_action.action == device::key_action::repeat) {
                if (key_action.key == device::keys::key_w) {
                    keyboardInput.moveForward = true;
                }
                if (key_action.key == device::keys::key_s) {
                    keyboardInput.moveBackward = true;
                }
                if (key_action.key == device::keys::key_a) {
                    keyboardInput.moveLeft = true;
                }
                if (key_action.key == device::keys::key_d) {
                    keyboardInput.moveRight = true;
                }
            }
            m_input.keyboardInput = keyboardInput;
        };

        auto resize_callback = [&](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::framebuffer>(args));
            const auto size = std::get<application::app_event::framebuffer>(args).size;
            m_perspective =
                glm::perspective(glm::radians(fov), static_cast<float>(size.width) / size.height, 0.01f, 10.0f);
        };

        auto mouse_callback = [&](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::mouse_pos>(args));
            const auto pos = std::get<application::app_event::mouse_pos>(args);
            m_input.mouseInput = {static_cast<std::int32_t>(pos.xpos), static_cast<std::int32_t>(pos.ypos)};
            m_input.newMousePos = true;
        };

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::keyboard>(),
            this,
            std::move(key_callback));

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::framebuffer>(),
            this,
            std::move(resize_callback));

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::mouse_pos>(),
            this,
            std::move(mouse_callback));
    }

    camera::camera(camera&& other) noexcept
        : m_name(std::move(other.m_name))
        , m_eventProcessor(other.m_eventProcessor)
        , m_buffer(std::move(other.m_buffer))
        , m_position(other.m_position)
        , m_direction(other.m_direction)
        , m_orientation(other.m_orientation)
        , m_perspective(other.m_perspective)
        , m_moveSensitivity(other.m_moveSensitivity)
        , m_rotateSensitivity(other.m_rotateSensitivity)
        , m_input(other.m_input) {
    }

    void camera::update(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) {
        const auto time = std::chrono::duration<float, std::chrono::seconds::period>(elapsedTime).count();

        if (m_input.newMousePos) {
            m_input.newMousePos = false;
            const auto yawDelta = calculate_yaw(time);
            const auto pitchDelta = calculate_pitch(time);
            m_orientation = pitchDelta * m_orientation * yawDelta;
            const auto mat = glm::toMat3(m_orientation);
            m_direction = glm::normalize(-glm::vec3(mat[0][2], mat[1][2], mat[2][2]));
        }
        m_position = caclulate_position(time);

        const auto orientationMartix = glm::translate(glm::toMat4(m_orientation), -m_position.xyz());

        m_buffer.update_buffer<uniforms>(
            {orientationMartix, m_perspective, m_perspective * orientationMartix, glm::mat4(1.0f)});
    }

    glm::vec4 camera::caclulate_position(float elapsedTime) const {
        glm::vec3 moveDir(0.0f, 0.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));
        if (m_input.keyboardInput.moveForward) {
            moveDir += m_direction;
        }
        if (m_input.keyboardInput.moveBackward) {
            moveDir -= m_direction;
        }
        if (m_input.keyboardInput.moveLeft) {
            moveDir -= right;
        }
        if (m_input.keyboardInput.moveRight) {
            moveDir += right;
        }

        const auto deltaPos = glm::vec4(moveDir * elapsedTime * m_moveSensitivity, 0.0f);

        return m_position + deltaPos;
    }

    glm::quat camera::calculate_pitch(float elapsedTime) const {
        const float pitchAngle = static_cast<float>(m_input.mouseInput.yPos) * elapsedTime * m_rotateSensitivity;
        return glm::angleAxis(pitchAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::quat camera::calculate_yaw(float elapsedTime) const {
        const float yawAngle = static_cast<float>(m_input.mouseInput.xPos) * elapsedTime * m_rotateSensitivity;
        return glm::angleAxis(yawAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 camera::calculate_direction(glm::quat pitch, glm::quat yaw) const {
        return glm::normalize(glm::rotate(glm::normalize(glm::inverse(pitch * yaw)), m_direction));
    }

} // namespace scene
} // namespace tst