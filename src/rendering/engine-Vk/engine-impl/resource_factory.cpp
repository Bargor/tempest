// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_factory.h"

#include "resource_cache.h"
#include "shader_compiler.h"

namespace tst {
namespace engine {
    namespace vulkan {

        resource_factory::resource_factory(device& device, application::data_loader& dataLoader)
            : m_device(device)
            , m_dataLoader(dataLoader)
            , m_resourceCache(m_device.get_resource_cache())
            , m_shaderCompiler(std::make_unique<shader_compiler>(m_dataLoader, m_device))
            , m_commandPool(m_device.create_command_pool()) {
        }

        resource_factory::~resource_factory() {
        }

        const pipeline* resource_factory::create_pipeline(base::pipeline_settings&& pipelineSettings,
                                                          const std::string& techniqueName,
                                                          const std::string& shadersName,
                                                          const vertex_format& format) {
            auto shaders = m_resourceCache.find_shaders(shadersName);
            auto technique = m_resourceCache.find_technique(techniqueName);

            if (!shaders) {
                shaders = load_shaders(shadersName);
            }

            if (shaders && technique) {
                auto pipeline = m_device.create_pipeline(std::move(pipelineSettings), format, *shaders, *technique);

                m_resourceCache.add_pipeline(std::move(pipeline));

                return m_resourceCache.find_pipeline();
            }
            return nullptr;
        }

        vertex_buffer resource_factory::create_vertex_buffer(const vertex_format& format, std::vector<vertex>&& vertices) {
            return m_device.create_vertex_buffer(format, std::move(vertices), m_commandPool);
        }

        uniform_buffer resource_factory::create_uniform_buffer() {
            return m_device.create_uniform_buffer(m_commandPool);
        }

        const shader_set* resource_factory::load_shaders(const std::string& shadersName) {
            auto shaders = m_shaderCompiler->compile_shaders(shadersName);
            m_resourceCache.add_shaders(shadersName, std::move(shaders));

            return m_resourceCache.find_shaders(shadersName);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
