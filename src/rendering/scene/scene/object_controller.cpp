// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "object_controller.h"

#include "scene.h"

#include <application/app_event.h>
#include <application/data_loader.h>
#include <application/event_processor.h>
#include <engine/resource_factory.h>
#include <util/variant.h>

namespace tst {
namespace scene {

    object_controller::object_controller(scene& scene,
                                         const application::data_loader& dataLoader,
                                         engine::resource_factory& resourceFactory)
        : m_scene(scene), m_dataLoader(dataLoader), m_resourceFactory(resourceFactory) {
    }

    void object_controller::load_object(const std::string& path) {
        m_dataLoader.find_file(path);
        auto vertexFormat = engine::vertex_format(engine::vertex_format::primitive_topology::triangle_list);
        vertexFormat.add_attribute(engine::vertex_format::location::position,
                                   engine::vertex_format::format::float2,
                                   offsetof(engine::vertex, pos),
                                   sizeof(engine::vertex),
                                   0);
        vertexFormat.add_attribute(engine::vertex_format::location::normal,
                                   engine::vertex_format::format::float3,
                                   offsetof(engine::vertex, color),
                                   sizeof(engine::vertex),
                                   0);

        m_resourceFactory.create_technique("test");
        const auto& pipeline = m_resourceFactory.create_pipeline("test", "test", vertexFormat);

        auto vertexBuffer =
            m_resourceFactory.create_vertex_buffer(vertexFormat,
                                                   std::vector<engine::vertex>({{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                                                                {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                                                                                {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                                                                                {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}}));
        auto indexBuffer = m_resourceFactory.create_index_buffer(std::vector<std::uint16_t>({{0, 1, 2, 2, 3, 0}}));

        auto uniformBuffer = m_resourceFactory.create_uniform_buffer("test", engine::shader::shader_type::vertex, 0);

        auto material = m_resourceFactory.create_material();

        scene_object object(
            std::move(vertexBuffer), std::move(indexBuffer), std::move(material), std::move(uniformBuffer), pipeline);
        m_scene.add_object(std::move(object));
    }

} // namespace scene
} // namespace tst
