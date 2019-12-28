// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_cache.h"

#include "swap_chain.h"

namespace tst {
namespace engine {
    namespace vulkan {

        resource_cache::resource_cache(const vk::Device device) : m_device(device) {
        }

        std::size_t resource_cache::add_pipeline(pipeline&& newPipeline) {
            auto hash = std::hash<pipeline>{}(newPipeline);
            m_pipelines.insert(std::make_pair(hash, std::move(newPipeline)));
            return hash;
        }

        void resource_cache::add_rendering_technique(rendering_technique&& technique) {
            m_techniques.emplace_back(std::move(technique));
        }

        void resource_cache::add_shaders(const std::string& name, shader_set&& shaders) {
            m_shaders.insert(std::make_pair(name, std::move(shaders)));
        }

        void resource_cache::add_descritptor_set_layout(shader::descriptor_layout&& layout,
                                                        vk::DescriptorSetLayout vkLayout) {
            if (m_descriptorLayouts.find(layout) == m_descriptorLayouts.cend()) {
                m_descriptorLayouts[layout] = vkLayout;
            }
        }

        const pipeline* resource_cache::find_pipeline(std::size_t pipelineHash) const {
            const auto& it = m_pipelines.find(pipelineHash);
            return (it != m_pipelines.end()) ? &(it->second) : nullptr;
        }

        const shader_set* resource_cache::find_shaders(const std::string& name) const {
            auto shaders = m_shaders.find(name);
            if (shaders != m_shaders.end()) {
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

        const vk::DescriptorSetLayout* resource_cache::find_descriptor_layout(const shader::descriptor_layout& layout) {
            const auto& flayout = m_descriptorLayouts.find(layout);
            if (flayout != m_descriptorLayouts.end()) {
                return &flayout->second;
            }
            return nullptr;
        }

        void resource_cache::clear() {
            m_pipelines.clear();
            m_techniques.clear();
            m_shaders.clear();
            for (const auto& layout : m_descriptorLayouts) {
                m_device.destroyDescriptorSetLayout(layout.second);
            }
            m_descriptorLayouts.clear();
        }

        void resource_cache::rebuild_techniques(const swap_chain& newSwapChain) {
            for (auto& technique : m_techniques) {
                technique.recreate_technique(newSwapChain);
            }
        }

        void resource_cache::rebuild_pipelines() {
            for (auto& pipeline : m_pipelines) {
                pipeline.second.recreate();
            }
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
