// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "scene_object.h"

#include <chrono>
#include <engine/draw_info.h>

template<typename T>
using ptr = std::unique_ptr<T>;

namespace tst {

namespace scene {

    class scene;
    class scene_controller;

    std::vector<scene_object::state> update_scene(const scene& scene,
                                                  std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    std::vector<engine::draw_info> prepare_draw_info(const std::vector<scene_object::state>& sceneState);

    class scene {
        friend std::vector<scene_object::state> update_scene(const scene& scene,
                                                             std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    public:
        scene(std::string&& sceneName);
        ~scene();

        void add_object(scene_object&& object);

    private:
        std::string m_sceneName;
        ptr<scene_controller> m_sceneController;
        std::vector<scene_object> m_objects;
    };

} // namespace scene
} // namespace tst
