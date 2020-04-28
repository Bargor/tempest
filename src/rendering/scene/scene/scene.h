// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "scene_object.h"

#include <chrono>
#include <engine/draw_info.h>

namespace tst {

namespace application {
    class data_loader;
    template<typename Event>
    class event_processor;

    struct app_event;
} // namespace application

namespace engine {
    class resource_factory;
}

namespace scene {

    class scene;
    class object_controller;

    std::vector<scene_object::state> update_scene(scene& scene,
                                                  std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    std::vector<engine::draw_info> prepare_draw_info(const std::vector<scene_object::state>& sceneState);

    class scene {
        friend std::vector<scene_object::state> update_scene(scene& scene,
                                                             std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    public:
        scene(std::string&& sceneName,
              application::data_loader& dataLoader,
              application::event_processor<application::app_event>& eventProcessor,
              engine::resource_factory& resourceFactory);
        ~scene();

        void add_object(scene_object&& object);
        object_controller& get_object_controller() const noexcept;

    private:
        std::string m_sceneName;
        ptr<object_controller> m_sceneObjectController;
        std::vector<scene_object> m_objects;
    };

} // namespace scene
} // namespace tst
