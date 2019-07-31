// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "scene_object.h"

#include <chrono>
#include <engine/draw_info.h>

namespace tst {

namespace scene {

    class scene;

    std::vector<scene_object::state> update_scene(const scene& scene,
                                                  std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    std::vector<engine::draw_info> prepare_draw_info(const std::vector<scene_object::state>& sceneState);

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