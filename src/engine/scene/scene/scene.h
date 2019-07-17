// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "scene_object.h"

#include <chrono>
#include <map>

namespace tst {

namespace scene {

    class scene;

    scene update_scene(const scene& scene, std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    class scene {
        friend scene update_scene(const scene& scene, std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    public:
        void add_object(const scene_object& object);

    private:
        std::vector<scene_object> m_objects;
    };

} // namespace scene
} // namespace tst