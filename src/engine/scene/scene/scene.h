// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "scene_object.h"

#include <chrono>

namespace tst {

namespace resources {
    class resource_factory;
}

namespace scene {

    class scene;

    std::vector<scene_object::state> update_scene(const scene& scene,
                                                  std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    class scene {
        template<typename T>
        using ptr = std::unique_ptr<T>;
        friend std::vector<scene_object::state> update_scene(const scene& scene,
                                                             std::chrono::duration<std::uint64_t, std::micro> elapsedTime);

    public:
        scene(engine::api::device& device);

        void add_object(scene_object&& object);

    private:
        ptr<resources::resource_factory> m_resourceFactory;
        std::vector<scene_object> m_objects;
    };

} // namespace scene
} // namespace tst