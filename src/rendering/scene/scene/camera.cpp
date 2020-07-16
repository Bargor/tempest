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
        , m_moveSensitivity(3.0f)
        , m_rotateSensitivity(0.01f)
        , m_input() {
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
            m_input.mouseInput = {static_cast<std::uint32_t>(pos.xpos), static_cast<std::uint32_t>(pos.ypos)};
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

    void camera::update(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) {
        m_position = caclulate_position(elapsedTime);
        m_orientation = calculate_orientation(elapsedTime);

        const auto orientationMartix = glm::translate(glm::toMat4(m_orientation), -m_position.xyz());

        m_buffer.update_buffer<uniforms>(
            {orientationMartix, m_perspective, orientationMartix * m_perspective, glm::mat4(1.0f)});
    }

    glm::vec4 camera::caclulate_position(std::chrono::duration<std::uint64_t, std::micro> elapsedTime) const {
        glm::vec4 moveDir(0.0f, 0.0f, 0.0f, 0.0f);
        if (m_input.keyboardInput.moveForward) {
            moveDir.z -= 1.0f;
        }
        if (m_input.keyboardInput.moveBackward) {
            moveDir.z += 1.0f;
        }
        if (m_input.keyboardInput.moveLeft) {
            moveDir.x -= 1.0f;
        }
        if (m_input.keyboardInput.moveRight) {
            moveDir.x += 1.0f;
        }
        float time = std::chrono::duration<float, std::chrono::seconds::period>(elapsedTime).count();

        return m_position + moveDir * time * m_moveSensitivity;
    }

    glm::quat camera::calculate_orientation(std::chrono::duration<std::uint64_t, std::micro>) const {
        return m_orientation;
    }
} // namespace scene
} // namespace tst