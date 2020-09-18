// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/uniform_storage.h>
#include <glm.h>

namespace tst {
namespace engine {
    namespace base {

        class view {
        public:
            struct uniforms : uniform_storage {
                glm::mat4 view;
                glm::mat4 perspective;
                glm::mat4 viewPerspective;
                glm::mat4 orientation;
                glm::vec4 position;
            };

        public:
            view(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up, float fov, float aspect) noexcept;

        public:
            bool operator==(const view& other) const noexcept;

        public:
            void set_perspective(float fov, float aspect) noexcept;
            void set_perspective(const glm::mat4& newMatrix) noexcept;

            void set_position(glm::vec3 newPos) noexcept;

            void move(glm::vec3 delta) noexcept;
            void rotate(glm::quat yaw, glm::quat pitch) noexcept;
            void rotate_move(glm::quat yaw, glm::quat pitch, glm::vec3 delta) noexcept;

            glm::vec3 right() const noexcept;
            glm::vec3 direction() const noexcept;

            glm::vec4 get_position() const noexcept;
            const glm::mat4& get_view() const noexcept;
            const glm::mat4& get_perspective() const noexcept;
            const glm::mat4& get_orientation() const noexcept;

            uniforms get_uniforms() const noexcept;

        private:
            void update_view() const noexcept;

        private:
            glm::vec4 m_position;
            glm::vec3 m_direction;
            glm::quat m_orientation;  // have to fix this
            glm::mat4 mutable m_view; // world to camera local matrix
            glm::mat4 mutable m_orientationMatrix;
            glm::mat4 m_perspective; // camera local to projection matrix;
            bool mutable m_viewMatrixDirty;
        };

        TST_INLINE bool view::operator==(const view& other) const noexcept {
            if (m_position == other.m_position && m_orientation == other.m_orientation &&
                m_perspective == other.m_perspective) {
                return true;
            }
            return false;
        }

        TST_INLINE void view::set_perspective(float fov, float aspect) noexcept {
            m_perspective = glm::perspective(glm::radians(fov), aspect, 0.01f, 100.0f);
            m_perspective[1][1] *= -1;
        }

        TST_INLINE void view::set_perspective(const glm::mat4& newMatrix) noexcept {
            m_perspective = newMatrix;
        }

        TST_INLINE void view::set_position(glm::vec3 newPos) noexcept {
            m_position = glm::vec4(newPos, 0.0f);
            m_viewMatrixDirty = true;
        }

        TST_INLINE void view::rotate(glm::quat yaw, glm::quat pitch) noexcept {
            m_orientation = pitch * m_orientation * yaw;
            const auto mat = glm::toMat3(m_orientation);
            m_direction = glm::normalize(-glm::vec3(mat[0][2], mat[1][2], mat[2][2]));
            m_viewMatrixDirty = true;
            // glm::normalize(glm::rotate(glm::normalize(glm::inverse(pitch * yaw)), m_direction));
        }

        TST_INLINE void view::move(glm::vec3 delta) noexcept {
            m_position += glm::vec4(delta, 0.0f);
            m_viewMatrixDirty = true;
        }

        TST_INLINE void view::rotate_move(glm::quat yaw, glm::quat pitch, glm::vec3 delta) noexcept {
            m_position += glm::vec4(delta, 0.0f);
            m_orientation = pitch * m_orientation * yaw;
            const auto mat = glm::toMat3(m_orientation);
            m_direction = glm::normalize(-glm::vec3(mat[0][2], mat[1][2], mat[2][2]));
            m_viewMatrixDirty = true;
        }

        TST_INLINE glm::vec3 view::right() const noexcept {
            return glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));
        }

        TST_INLINE glm::vec3 view::direction() const noexcept {
            return m_direction;
        }

        TST_INLINE glm::vec4 view::get_position() const noexcept {
            return m_position;
        }

        TST_INLINE const glm::mat4& view::get_view() const noexcept {
            if (m_viewMatrixDirty) {
                update_view();
            }
            return m_view;
        }

        TST_INLINE const glm::mat4& view::get_perspective() const noexcept {
            return m_perspective;
        }

        TST_INLINE const glm::mat4& view::get_orientation() const noexcept {
            if (m_viewMatrixDirty) {
                update_view();
            }
            return m_orientationMatrix;
        }

        TST_INLINE view::uniforms view::get_uniforms() const noexcept {
            return uniforms{{}, m_view, m_perspective, m_perspective * m_view, m_orientationMatrix, m_position};
        }

    } // namespace base
} // namespace engine
} // namespace tst