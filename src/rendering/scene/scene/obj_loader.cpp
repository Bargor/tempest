// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "obj_loader.h"

#include <application/data_loader.h>
#include <engine/resource_factory.h>
#include <fmt/printf.h>
#include <unordered_map>

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(push)
#pragma warning(disable : 4324)
#endif

namespace {
std::tuple<std::vector<tst::engine::vertex>, std::vector<std::uint32_t>>
load_mesh(const tst::application::data_loader& dataLoader, const std::filesystem::path& path) {
    const auto data = dataLoader.load_obj_model(path);

    std::unordered_map<tst::engine::vertex, std::uint32_t> uniqueVertices;

    std::vector<tst::engine::vertex> vertices;
    std::vector<std::uint32_t> indices;

    for (const auto& shape : data.shapes) {
        for (const auto& index : shape.mesh.indices) {
            tst::engine::vertex vertex;

            vertex.pos = {data.attrib.vertices[3 * index.vertex_index],
                          data.attrib.vertices[3 * index.vertex_index + 1],
                          data.attrib.vertices[3 * index.vertex_index + 2]};

            if (index.texcoord_index != -1) {
                vertex.texCoord = {data.attrib.texcoords[2 * index.texcoord_index],
                                   1.0f - data.attrib.texcoords[2 * index.texcoord_index + 1]};
            } else {
                vertex.texCoord = {0.0f, 0.0f};
            }

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<std::uint32_t>(vertices.size());
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
    engine::mesh load_obj_model(const application::data_loader& dataLoader,
                                engine::resource_factory& factory,
                                const std::filesystem::path& path) {
        auto [vertices, indices] = load_mesh(dataLoader, path);

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

        fmt::printf("Loaded model %s, vertices: %d triangles: %d\n", path.string(), vertices.size(), indices.size() / 3);

        auto create_buffer = [&factory, vertices = std::ref(vertices)](std::vector<std::uint32_t>&& indices) {
            if (vertices.get().size() >= std::numeric_limits<std::uint16_t>::max()) {
                return factory.create_index_buffer(std::move(indices));
            } else {
                std::vector<std::uint16_t> newIndices(indices.size());
                for (std::uint32_t idx = 0; idx < indices.size(); ++idx) {
                    newIndices[idx] = static_cast<std::uint16_t>(indices[idx]);
                }
                return factory.create_index_buffer(std::move(newIndices));
            }
        };

        auto indexBuffer = create_buffer(std::move(indices));
        auto vertexBuffer = factory.create_vertex_buffer(vertexFormat, std::move(vertices));

        return engine::mesh(std::move(vertexBuffer), std::move(indexBuffer), nullptr);
    }
} // namespace scene
} // namespace tst

#if TST_COMPILER & TST_COMPILER_VC
#pragma warning(pop)
#endif