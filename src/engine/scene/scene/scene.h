// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "scene_object.h"

#include <map>

namespace tst {

namespace scene {

    class scene;

    scene update_scene(const scene& scene);

    class scene {
        friend scene update_scene(const scene& scene);

    public:
        void add_object(scene_object& object);

    private:
        std::vector<scene_object> m_objects;
    };

} // namespace scene
} // namespace tst