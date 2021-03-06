// This file is part of Tempest-engine project
// Author: Karol Kontny

#pragma once

#include "camera.h"
#include "scene_object.h"

namespace tst {

namespace application {
    class data_loader;
} // namespace application

namespace engine {
    class resource_factory;
}

namespace scene {

    class object_controller {
    public:
        object_controller(const application::data_loader& dataLoader,
                          engine::resource_factory& resourceFactory);

        scene_object load_object(const std::string_view objectName, const std::string_view shaderName);

    private:
        engine::model load_model(std::string_view path);
        engine::model load_dummy_model();

    private:
        const application::data_loader& m_dataLoader;
        engine::resource_factory& m_resourceFactory;
    };

} // namespace scene
} // namespace tst
