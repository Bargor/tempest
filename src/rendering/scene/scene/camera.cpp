// This file is part of Tempest project
// Author: Karol Kontny

#include "camera.h"

#include <application/app_event.h>
#include <application/event_processor.h>
#include <device/keyboard.h>
#include <util/variant.h>

namespace tst {
namespace scene {

    camera::camera(std::string cameraName,
                   application::event_processor<application::app_event>& eventProcessor,
                   const glm::vec3& position,
                   const glm::vec3& lookAt,
                   const glm::vec3& up,
                   float fov,
                   float aspect,
                   bool startSubscribed)
        : m_name(std::move(cameraName))
        , m_eventProcessor(eventProcessor)
        , m_view(position, lookAt, up, fov, aspect)
        , m_moveSensitivity(3.0f)
        , m_rotateSensitivity(0.5f)
        , m_input() {
        auto resize_callback = [&, fov, aspect](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::framebuffer>(args));
            const auto size = std::get<application::app_event::framebuffer>(args).size;
            if (size.height > 0) {
                m_view.set_perspective(fov, aspect);
            }
        };

        auto cursor_callback = [&](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::cursor>(args));
            const auto mode = std::get<application::app_event::cursor>(args);
            if (mode.cursor == application::window::cursor_mode::disabled) {
                subscribe();
            } else {
                unsubscribe();
            }
        };

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::framebuffer>(),
            this,
            std::move(resize_callback));

        m_eventProcessor.subscribe(
            core::variant_index<application::app_event::arguments, application::app_event::cursor>(),
            this,
            std::move(cursor_callback));

        if (startSubscribed) {
            subscribe();
        } 
    }

    camera::camera(camera&& other) noexcept
        : m_name(std::move(other.m_name))
        , m_eventProcessor(other.m_eventProcessor)
        , m_view(other.m_view)
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
            m_view.rotate_move(yawDelta, pitchDelta, caclulate_position_delta(time));
        } else {
            m_view.move(caclulate_position_delta(time));
        }
    }

    glm::vec4 camera::caclulate_position_delta(float elapsedTime) const {
        glm::vec3 moveDir(0.0f, 0.0f, 0.0f);
        glm::vec3 right = m_view.right();
        if (m_input.keyboardInput.moveForward) {
            moveDir += m_view.direction();
        }
        if (m_input.keyboardInput.moveBackward) {
            moveDir -= m_view.direction();
        }
        if (m_input.keyboardInput.moveLeft) {
            moveDir -= right;
        }
        if (m_input.keyboardInput.moveRight) {
            moveDir += right;
        }

        return glm::vec4(moveDir * elapsedTime * m_moveSensitivity, 0.0f);
    }

    glm::quat camera::calculate_pitch(float elapsedTime) const {
        const float pitchAngle = static_cast<float>(m_input.mouseInput.yPos) * elapsedTime * m_rotateSensitivity;
        return glm::angleAxis(pitchAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::quat camera::calculate_yaw(float elapsedTime) const {
        const float yawAngle = static_cast<float>(m_input.mouseInput.xPos) * elapsedTime * m_rotateSensitivity;
        return glm::angleAxis(yawAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void camera::subscribe() {
        auto key_callback = [this](const application::app_event::arguments& args) {
            assert(std::holds_alternative<application::app_event::keyboard>(args));
            application::app_event::keyboard key_action = std::get<application::app_event::keyboard>(args);
            if (key_action.action == device::key_action::press || key_action.action == device::key_action::repeat) {
                if (key_action.key == device::keys::key_w) {
                    m_input.keyboardInput.moveForward = true;
                }
                if (key_action.key == device::keys::key_s) {
                    m_input.keyboardInput.moveBackward = true;
                }
                if (key_action.key == device::keys::key_a) {
                    m_input.keyboardInput.moveLeft = true;
                }
                if (key_action.key == device::keys::key_d) {
                    m_input.keyboardInput.moveRight = true;
                }
            }
            if (key_action.action == device::key_action::release) {
                if (key_action.key == device::keys::key_w) {
                    m_input.keyboardInput.moveForward = false;
                }
                if (key_action.key == device::keys::key_s) {
                    m_input.keyboardInput.moveBackward = false;
                }
                if (key_action.key == device::keys::key_a) {
                    m_input.keyboardInput.moveLeft = false;
                }
                if (key_action.key == device::keys::key_d) {
                    m_input.keyboardInput.moveRight = false;
                }
            }
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
            core::variant_index<application::app_event::arguments, application::app_event::mouse_pos>(),
            this,
            std::move(mouse_callback));
    }

    void camera::unsubscribe() {
        m_eventProcessor.unsubscribe(
            core::variant_index<application::app_event::arguments, application::app_event::keyboard>(), this);
        m_eventProcessor.unsubscribe(
            core::variant_index<application::app_event::arguments, application::app_event::mouse_pos>(), this);
    }

} // namespace scene
} // namespace tst