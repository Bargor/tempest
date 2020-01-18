// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "shader_compiler.h"

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

        const pipeline& resource_factory::create_pipeline(const std::string&, const std::string&, const vertex_format&) {
            std::vector<pipeline> m_pipe;
            pipeline p{};
            m_pipe.push_back(p);
            return m_pipe[0];
        }

        vertex_buffer resource_factory::create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) {
            return vertex_buffer(format, std::move(vertices));
        }

        uniform_buffer resource_factory::create_uniform_buffer(const std::string&, shader_type, std::uint32_t) {
            return uniform_buffer();
        }

        void resource_factory::create_technique(std::string&& name) {
            m_shaderCompiler->compile_program(name, shaderTypesSet{});
        }
    } // namespace opengl
} // namespace engine
} // namespace tst