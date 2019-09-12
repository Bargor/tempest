// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "object_controller.h"

#include "scene.h"

#include <application/data_loader.h>
#include <application/event_processor.h>
#include <engine/resource_factory.h>

namespace tst {
namespace scene {

    object_controller::object_controller(scene& scene,
                                         application::data_loader& dataLoader,
                                         application::event_processor<application::app_event>& eventProcessor,
                                         engine::resource_factory& resourceFactory)
        : m_scene(scene), m_dataLoader(dataLoader), m_eventProcessor(eventProcessor), m_resourceFactory(resourceFactory) {
    }

    scene_object object_controller::load_object(const std::string&) {
        auto vertexBuffer =
            m_resourceFactory.create_vertex_buffer(engine::vertex_format{},
                                                   std::vector<engine::vertex>({{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                                                {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                                                {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                                                {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}}));
        auto indexBuffer = m_resourceFactory.create_index_buffer(std::vector<std::uint16_t>({{0, 1, 2, 2, 3, 0}}));

        scene_object object(std::move(vertexBuffer), std::move(indexBuffer));
        m_scene.add_object(std::move(object));
    }

} // namespace scene
} // namespace tst
