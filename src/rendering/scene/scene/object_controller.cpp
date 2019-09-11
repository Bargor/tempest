// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "object_controller.h"
#include "scene.h"

#include <application/data_loader.h>
#include <application/event_processor.h>

namespace tst {
namespace scene {

    object_controller::object_controller(scene& scene,
                                         application::data_loader& dataLoader,
                                         application::event_processor<application::app_event>& eventProcessor)
        : m_scene(scene), m_dataLoader(dataLoader), m_eventProcessor(eventProcessor) {
    }

    scene_object object_controller::load_object(std::string shaderName) {
    }

} // namespace scene
} // namespace tst
