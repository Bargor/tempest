// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace tst {
namespace scene {
    class camera {
    public:
    private:
        glm::vec3 m_position;
        glm::quat m_orientation;
    };
} // namespace scene
} // namespace tst