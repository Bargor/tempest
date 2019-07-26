// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "scene_object.h"

#include <chrono>

namespace tst {

namespace scene {

    class scene;

    std::vector<scene_object::state> update_scene(const scene& scene,
                                                  std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    class scene {
        friend std::vector<scene_object::state> update_scene(const scene& scene,
                                                             std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    public:
        scene();
        ~scene();

        void add_object(scene_object&& object);

    private:
        std::vector<scene_object> m_objects;
    };

} // namespace scene
} // namespace tst