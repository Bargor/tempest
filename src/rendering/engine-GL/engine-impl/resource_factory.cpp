// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "shader_compiler.h"

#include <application/data_loader.h>
#include <fmt/format.h>

namespace tst {
namespace engine {
    namespace opengl {
        resource_factory::resource_factory(const application::data_loader& dataLoader)
            : m_dataLoader(dataLoader), m_shaderCompiler(std::make_unique<shader_compiler>(m_dataLoader)) {
        }

        resource_factory::~resource_factory() {
        }

        resource_factory::resource_factory(resource_factory&& factory) noexcept
            : m_dataLoader(factory.m_dataLoader), m_shaderCompiler(std::move(factory.m_shaderCompiler)) {
        }

        const pipeline& resource_factory::create_pipeline(const std::string&,
                                                          const std::string&,
                                                          const std::string&,
                                                          const vertex_format&) {
            pipeline p{};
            m_pipelines.push_back(p);
            return m_pipelines[0];
        }

        texture resource_factory::create_texture(const std::string& textureName) {
            const auto textureFile = m_dataLoader.find_file(std::filesystem::path("textures") / (textureName));
            if (!textureFile) {
                throw std::runtime_error(fmt::format("Wrong texture path: so such file: %s", "textures/" + textureName));
            }
            return texture(m_dataLoader.load_image(textureFile.value()));
        }

        vertex_buffer resource_factory::create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) {
            return vertex_buffer(format, std::move(vertices));
        }

        uniform_buffer resource_factory::create_uniform_buffer(const std::string&,
                                                               base::resource_bind_point,
                                                               std::uint32_t,
                                                               std::size_t storageSize) {
            return uniform_buffer(storageSize);
        }

        material resource_factory::create_material(const std::string materialName,
                                 const std::string&,
                                 const std::vector<std::string>&,
                                 std::uint32_t,
                                 std::uint32_t) {
            return material(materialName);
        }

        void resource_factory::create_technique(const std::string& name) {
            m_shaderCompiler->compile_program(name, shaderTypesSet{});
        }
    } // namespace opengl
} // namespace engine
} // namespace tst