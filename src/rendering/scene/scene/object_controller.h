// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "scene_object.h"

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

    class object_controller {
    public:
        object_controller(scene& scene,
                          const application::data_loader& dataLoader,
                          engine::resource_factory& resourceFactory);

        void load_object(const std::string& shaderName);

    private:
        scene& m_scene;
        const application::data_loader& m_dataLoader;
        engine::resource_factory& m_resourceFactory;
    };

} // namespace scene
} // namespace tst
