// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "shader_compiler.h"

namespace tst {
namespace engine {
    namespace opengl {
        resource_factory::resource_factory(device& device, application::data_loader& dataLoader)
            : m_device(device)
            , m_dataLoader(dataLoader)
            , m_shaderCompiler(std::make_unique<shader_compiler>(m_dataLoader)) {
        }

        resource_factory::~resource_factory() {
        }

        const pipeline& resource_factory::create_pipeline(const std::string&, const std::string&, const vertex_format&) {
            std::vector<pipeline> m_pipe;
            pipeline p{};
            m_pipe.push_back(p);
            return m_pipe[0];
        }

        vertex_buffer resource_factory::create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) {
            return m_device.create_vertex_buffer(format, std::move(vertices));
        }

        uniform_buffer resource_factory::create_uniform_buffer(const std::string& shaderName, shader_type, std::uint32_t) {
            return m_device.create_uniform_buffer(shaderName);
        }

        void resource_factory::create_technique(std::string&& name, base::technique_settings&&) {
            m_shaderCompiler->compile_program(name, shaderTypesSet{});
        }
    } // namespace opengl
} // namespace engine
} // namespace tst