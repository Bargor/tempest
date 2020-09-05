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

        pipeline::pipeline_hash resource_factory::create_pipeline(const std::string&,
                                                                  std::string_view,
                                                                  const std::string&,
                                                                  const vertex_buffer&) {
            pipeline p{};
            m_pipelines.push_back(p);
            return 1;
        }

        texture resource_factory::create_texture(const std::string& textureName) {
            const auto textureFile = m_dataLoader.find_file(std::filesystem::path("textures") / (textureName));
            if (!textureFile) {
                throw std::runtime_error(fmt::format("Wrong texture path: so such file: %s", "textures/" + textureName));
            }
            return texture(m_dataLoader.load_image(textureFile.value()));
        }

        uniform_buffer resource_factory::create_uniform_buffer(const std::string& shaderName,
                                                               base::resource_bind_point bindPoint,
                                                               std::uint32_t binding,
                                                               std::size_t storageSize) {
            return uniform_buffer(create_uniform_creation_info(shaderName, bindPoint), binding, storageSize);
        }

        material resource_factory::create_material(std::string&& materialName,
                                                   const std::string& shaderName,
                                                   const std::vector<std::string>& textureNames,
                                                   std::uint32_t staticStorageSize,
                                                   std::uint32_t dynamicStorageSize) {
            return material(std::move(materialName), shaderName, textureNames, staticStorageSize, dynamicStorageSize);
        }

        void resource_factory::create_technique(std::string&& name) {
            m_shaderCompiler->compile_program(name, shaderTypesSet{});
        }

        buffer::creation_info resource_factory::create_buffer_creation_info() const noexcept {
            return buffer::creation_info{};
        }

        uniform_buffer::creation_info resource_factory::create_uniform_creation_info(
            const std::string&, base::resource_bind_point) const noexcept {
            return uniform_buffer::creation_info{};
        }
    } // namespace opengl
} // namespace engine
} // namespace tst