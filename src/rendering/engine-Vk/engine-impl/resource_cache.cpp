// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_cache.h"

#include "resources/pipeline.h"

namespace tst {
namespace engine {
    namespace vulkan {

        void resource_cache::add_pipeline(pipeline& pipeline) {
            m_pipelines.insert(pipeline);
        }

        shader_set* resource_cache::find_shaders(const std::string& name) const {
            auto shaders = m_shaders.find(name);
            if (shaders == m_shaders.end()) {
                return &(shaders->second);
            }
            return nullptr;
        }

        rendering_technique* resource_cache::find_technique(const std::string& techniqueName) const {
            auto technique =
                std::find_if(m_techniques.begin(), m_techniques.end(), [&](const rendering_technique& technique) {
                    if (technique.get_name() == techniqueName) {
                        return true;
                    }
                    return false;
                });

            if (technique != m_techniques.end()) {
                return technique;
            }
            return nullptr;
        }

        void resource_cache::add_shaders(std::string& name, shader_set&& shaders) {
            m_shaders.insert(std::make_pair(name, std::move(shaders)));
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
