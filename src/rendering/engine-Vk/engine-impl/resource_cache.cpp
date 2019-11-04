// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_cache.h"

#include "resources/pipeline.h"

namespace tst {
namespace engine {
    namespace vulkan {

        std::size_t resource_cache::add_pipeline(pipeline&& newPipeline) {
            auto hash = std::hash<pipeline>{}(newPipeline);
            m_pipelines.insert(std::make_pair(hash, std::move(newPipeline)));
            return hash;
        }

        void resource_cache::add_shaders(const std::string& name, shader_set&& shaders) {
            m_shaders.insert(std::make_pair(name, std::move(shaders)));
        }

        const pipeline* resource_cache::find_pipeline(std::size_t pipelineHash) const {
            const auto& it = m_pipelines.find(pipelineHash);
            return (it != m_pipelines.end()) ? &(it->second) : nullptr;
        }

        const shader_set* resource_cache::find_shaders(const std::string& name) const {
            auto shaders = m_shaders.find(name);
            if (shaders == m_shaders.end()) {
                return &(shaders->second);
            }
            return nullptr;
        }

        const rendering_technique* resource_cache::find_technique(const std::string& techniqueName) const {
            auto technique =
                std::find_if(m_techniques.begin(), m_techniques.end(), [&](const rendering_technique& technique) {
                    if (technique.get_name() == techniqueName) {
                        return true;
                    }
                    return false;
                });

            if (technique != m_techniques.end()) {
                return &(*technique);
            }
            return nullptr;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
