// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "object_controller.h"

#include <application/app_event.h>
#include <application/data_loader.h>
#include <application/event_processor.h>
#include <engine/resource_factory.h>
#include <util/variant.h>

namespace tst {
namespace scene {

    object_controller::object_controller(const application::data_loader& dataLoader,
                                         application::event_processor<application::app_event>& eventProcessor,
                                         engine::resource_factory& resourceFactory)
        : m_dataLoader(dataLoader), m_eventProcessor(eventProcessor), m_resourceFactory(resourceFactory) {
    }

    scene_object object_controller::load_object(const std::string& objectName, const std::string& path) {
        m_dataLoader.find_file(path);
        auto vertexFormat = engine::vertex_format(engine::vertex_format::primitive_topology::triangle_list);
        vertexFormat.add_attribute(engine::vertex_format::location::position,
                                   engine::vertex_format::format::float3,
                                   offsetof(engine::vertex, pos),
                                   sizeof(engine::vertex),
                                   0);
        vertexFormat.add_attribute(engine::vertex_format::location::normal,
                                   engine::vertex_format::format::float3,
                                   offsetof(engine::vertex, color),
                                   sizeof(engine::vertex),
                                   0);
        vertexFormat.add_attribute(engine::vertex_format::location::texcoord,
                                   engine::vertex_format::format::float2,
                                   offsetof(engine::vertex, texCoord),
                                   sizeof(engine::vertex),
                                   0);

        m_resourceFactory.create_technique("test");
        const auto& pipeline = m_resourceFactory.create_pipeline("test", "test", "test", vertexFormat);

        auto vertexBuffer = m_resourceFactory.create_vertex_buffer(
            vertexFormat,
            std::vector<engine::vertex>({{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                         {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                         {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                         {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
                                         {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                         {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                         {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                         {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}}));
        auto indexBuffer =
            m_resourceFactory.create_index_buffer(std::vector<std::uint16_t>({0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4}));

        auto uniformBuffer = m_resourceFactory.create_uniform_buffer<engine::resources::uniform_buffer_object>(
            "test", engine::shader::shader_type::vertex, 0);
        auto texture = m_resourceFactory.create_texture("texture.jpg");
        texture.bind_texture("test", engine::shader::shader_type::fragment, 0);

        auto material = m_resourceFactory.create_material();

        return scene_object(objectName,
                            std::move(vertexBuffer),
                            std::move(indexBuffer),
                            std::move(material),
                            std::move(uniformBuffer),
                            std::move(texture),
                            pipeline);
    }

    camera object_controller::create_camera(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up) {
        auto buffer = m_resourceFactory.create_uniform_buffer<glm::mat4>("test", engine::shader::shader_type::vertex, 0);
        return camera(m_eventProcessor, std::move(buffer), position, lookAt, up);
    }

} // namespace scene
} // namespace tst
