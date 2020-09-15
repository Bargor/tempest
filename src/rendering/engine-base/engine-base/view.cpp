// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "view.h"

namespace tst {
namespace engine {
    namespace base {

        view::view(const glm::vec3& position,
                   const glm::vec3& lookAt,
                   const glm::vec3& up,
                   const float fov,
                   const float aspect) noexcept
            : m_position(glm::vec4(position, 0.0f))
            , m_direction(glm::normalize(lookAt - position))
            , m_orientation(glm::quatLookAt(m_direction, up))
            , m_view(glm::translate(glm::toMat4(m_orientation), -position))
            , m_orientationMatrix(glm::inverse(m_view))
            , m_perspective(glm::perspective(glm::radians(fov), aspect, 0.01f, 100.0f))
            , m_viewMatrixDirty(false) {
            m_perspective[1][1] *= -1;
        }

        void view::update_view() const noexcept {
            m_view = glm::translate(glm::toMat4(m_orientation), -m_position.xyz());
            m_orientationMatrix = glm::inverse(m_view);
            m_viewMatrixDirty = false;
        }

    } // namespace base
} // namespace engine
} // namespace tst