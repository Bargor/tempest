// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "obj_loader.h"

#include <engine/resource_factory.h>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(push)
#pragma warning(disable : 4324) // nonstandard extension used : nameless struct/union
#endif

namespace {
std::tuple<std::vector<tst::engine::vertex>, std::vector<std::uint16_t>> load_mesh(const std::filesystem::path& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.string().c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<tst::engine::vertex, std::uint16_t> uniqueVertices;

    std::vector<tst::engine::vertex> vertices;
    std::vector<std::uint16_t> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            tst::engine::vertex vertex;

            vertex.pos = {attrib.vertices[3 * index.vertex_index + 0],
                          attrib.vertices[3 * index.vertex_index + 1],
                          attrib.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = {attrib.texcoords[2 * index.texcoord_index + 0],
                               1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<std::uint16_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
    return std::tie(vertices, indices);
}
} // namespace

namespace tst {
namespace scene {
    engine::mesh load_obj_model(engine::resource_factory& factory, const std::filesystem::path& path) {
        auto [vertices, indices] = load_mesh(path);

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
        auto vertexBuffer = factory.create_vertex_buffer(vertexFormat, std::move(vertices));
        auto indexBuffer = factory.create_index_buffer(std::move(indices));

        return engine::mesh(std::move(vertexBuffer), std::move(indexBuffer), nullptr);
    }
} // namespace scene
} // namespace tst

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(pop)
#endif