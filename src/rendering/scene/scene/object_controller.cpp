// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "object_controller.h"

#include "obj_loader.h"

#include <application/data_loader.h>
#include <engine/resource_factory.h>
#include <materials/test.h>
#include <util/variant.h>

namespace tst {
namespace scene {

    object_controller::object_controller(const application::data_loader& dataLoader,
                                         engine::resource_factory& resourceFactory)
        : m_dataLoader(dataLoader), m_resourceFactory(resourceFactory) {
    }

    scene_object object_controller::load_object(std::string_view objectName, std::string_view path) {

        engine::model model = load_model(path);

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
            std::vector<engine::vertex>({{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
                                         {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                                         {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                                         {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                                         {{-2.5f, -1.5f, -2.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                         {{-1.5f, -1.5f, -2.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                         {{-1.5f, -0.5f, -2.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                         {{-2.5f, -0.5f, -2.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}}));
        auto indexBuffer = m_resourceFactory.create_index_buffer(
            std::vector<std::uint16_t>({0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8}));

        auto uniformBuffer =
            m_resourceFactory.create_uniform_buffer<uniform_buffer_object>("test", engine::bind_point::global_static, 0);

        auto material = m_resourceFactory.create_material<materials::test_material>("test", "test", {"texture.jpg"});

        return scene_object(std::string(objectName),
                            std::move(vertexBuffer),
                            std::move(indexBuffer),
                            std::move(material),
                            std::move(uniformBuffer),
                            pipeline);
    }

    scene_object object_controller::load_object2(std::string_view objectName, std::string_view path) {
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
            std::vector<engine::vertex>({{{0.5f, 0.5f, 7.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
                                         {{-0.5f, 0.5f, 7.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
                                         {{-0.5f, -0.5f, 7.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
                                         {{0.5f, -0.5f, 7.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}}));
        auto indexBuffer = m_resourceFactory.create_index_buffer(
            std::vector<std::uint16_t>({0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8}));

        auto uniformBuffer =
            m_resourceFactory.create_uniform_buffer<uniform_buffer_object>("test", engine::bind_point::global_static, 0);

        auto material = m_resourceFactory.create_material<materials::test_material>("test", "test", {"texture.jpg"});

        return scene_object(std::string(objectName),
                            std::move(vertexBuffer),
                            std::move(indexBuffer),
                            std::move(material),
                            std::move(uniformBuffer),
                            pipeline);
    }

    engine::model object_controller::load_model(std::string_view path) {
        
        const auto filepath = m_dataLoader.find_file(path);
        if (filepath && filepath->extension() == ".obj") {
            std::vector<engine::mesh> meshes;
            meshes.emplace_back(load_obj_model(m_resourceFactory, *filepath));
            
            return engine::model(std::move(meshes), {});
        } else {
            return load_dummy_model();
        }
    }

    engine::model object_controller::load_dummy_model() {
        return engine::model();
    }

} // namespace scene
} // namespace tst
