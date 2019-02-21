// This file is part of Tempest-engine-GL project
// Author: Karol Kontny

#include "scene.h"
#include <application/data_loader.h>

namespace tst {
namespace engine {
namespace scene {

    scene::scene(application::data_loader& dataLoader) : m_dataLoader(dataLoader) {
    }

    scene::~scene() {

    }

    void scene::load_model() { 
        m_dataLoader.load_obj_model(std::string_view("asd"));
    }

} // namespace scene
} // namespace engine
} // namespace tst