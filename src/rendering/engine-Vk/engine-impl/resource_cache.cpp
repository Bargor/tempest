// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_cache.h"

#include "swap_chain.h"

namespace tst {
namespace engine {
    namespace vulkan {

        resource_cache::resource_cache(const vk::Device device) : m_device(device) {
            create_descriptor_pool(400);
        }

        resource_cache::~resource_cache() {
            destroy();
        }

        std::size_t resource_cache::add_pipeline(pipeline&& newPipeline) {
            auto hash = std::hash<pipeline>{}(newPipeline);
            m_pipelines.insert({hash, std::move(newPipeline)});
            return hash;
        }

        void resource_cache::add_rendering_technique(rendering_technique&& technique) {
            m_techniques.emplace_back(std::move(technique));
        }

        void resource_cache::add_shaders(const std::string& name, shader_set&& shaders) {
            for (const auto layout : shaders.layouts) {
                std::vector<vk::DescriptorSetLayout> layouts(settings::m_inFlightFrames, layout);
                const vk::DescriptorSetAllocateInfo allocInfo(
                    m_descriptorPools[0], settings::m_inFlightFrames, layouts.data());

                const auto sets = m_device.allocateDescriptorSets(allocInfo);

                descriptor_set descSets;

                std::copy_n(sets.begin(), settings::m_inFlightFrames, descSets.begin());

                m_descriptorSets[name].push_back(descSets);
            }

            m_shaders.insert({name, std::move(shaders)});
        }

        vk::DescriptorPool resource_cache::create_descriptor_pool(std::uint32_t) {
            vk::DescriptorPoolSize uniformPoolSize(vk::DescriptorType::eUniformBuffer, settings::m_inFlightFrames* 10);
            vk::DescriptorPoolSize samplerPoolSize(vk::DescriptorType::eCombinedImageSampler, settings::m_inFlightFrames * 3);

            vk::DescriptorPoolCreateInfo poolCreateInfo(
                vk::DescriptorPoolCreateFlags(),
                settings::m_inFlightFrames * 3 * 5,
                2,
                std::array<vk::DescriptorPoolSize, 2>{uniformPoolSize, samplerPoolSize}.data());

            m_descriptorPools.emplace_back(m_device.createDescriptorPool(poolCreateInfo));

            return m_descriptorPools.back();
        }

        const pipeline* resource_cache::find_pipeline(std::size_t pipelineHash) const {
            const auto& it = m_pipelines.find(pipelineHash);
            return (it != m_pipelines.end()) ? &(it->second) : nullptr;
        }

        const shader_set* resource_cache::find_shaders(const std::string& name) const {
            const auto shaders = m_shaders.find(name);
            if (shaders != m_shaders.end()) {
                return &(shaders->second);
            }
            return nullptr;
        }

        const rendering_technique* resource_cache::find_technique(const std::string& techniqueName) const {
            const auto technique =
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

        const std::vector<vk::DescriptorSetLayout>*
        resource_cache::find_descriptor_layouts(const std::string& shadersName) const noexcept {
            const auto shaders = m_shaders.find(shadersName);
            if (shaders != m_shaders.end()) {
                return &shaders->second.layouts;
            }
            return nullptr;
        }

        const descriptor_set* resource_cache::find_descriptor_sets(const std::string& shadersName,
                                                                   base::resource_bind_point bindPoint) const {
            const auto descriptors = m_descriptorSets.find(shadersName);
            return &descriptors->second[static_cast<std::uint32_t>(bindPoint)];
        }

        void resource_cache::clear() {
            m_pipelines.clear();
            m_techniques.clear();
            for (const auto& shader : m_shaders) {
                for (const auto layout : shader.second.layouts) {
                    m_device.destroyDescriptorSetLayout(layout);
                }
            }
            m_shaders.clear();
        }

        void resource_cache::destroy() {
            clear();
            for (auto& descriptorPool : m_descriptorPools) {
                m_device.destroyDescriptorPool(descriptorPool);
            }
            m_descriptorPools.clear();
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
