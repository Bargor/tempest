// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "resource_cache.h"

namespace tst {
namespace engine {
    namespace vulkan {

        namespace {
            vk::DescriptorSetLayout create_global_layout(vk::Device device) {
                std::vector<vk::DescriptorSetLayoutBinding> bindings;

                bindings.emplace_back(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eAll, nullptr);

                vk::DescriptorSetLayoutCreateInfo setLayoutInfo(
                    vk::DescriptorSetLayoutCreateFlags(), static_cast<std::uint32_t>(bindings.size()), bindings.data());

                return device.createDescriptorSetLayout(setLayoutInfo, nullptr);
            }

        } // namespace

        resource_cache::resource_cache(const vk::Device device)
            : m_device(device)
            , m_descriptorPools({create_descriptor_pool(30, 25, 5), create_descriptor_pool(100, 100, 100)})
            , m_globalLayout(create_global_layout(device))
            , m_globalStaticSet(create_descriptor_set(m_globalLayout))
            , m_globalDynamicSet(create_descriptor_set(m_globalLayout))
            , m_viewStaticSet(create_descriptor_set(m_globalLayout))
            , m_viewDynamicSet(create_descriptor_set(m_globalLayout)) {
        }

        resource_cache::~resource_cache() {
            destroy();
        }

        std::size_t resource_cache::add_pipeline(pipeline&& newPipeline) {
            auto hash = std::hash<pipeline>{}(newPipeline);
            m_pipelines.insert({hash, std::move(newPipeline)});
            return hash;
        }

        void resource_cache::add_rendering_technique(std::string&& techniqueName,
                                                     base::technique_settings&& settings,
                                                     vk::Device device,
                                                     const swap_chain& swapChain) {
            m_techniques.emplace_back(std::move(techniqueName), std::move(settings), device, swapChain);
        }

        void resource_cache::add_shaders(std::string name, shader_set&& shaders) {
            for (const auto layout : shaders.layouts) {
                m_descriptorSets[name].push_back(create_descriptor_set(layout));
            }

            m_shaders.insert({std::move(name), std::move(shaders)});
        }

        vk::DescriptorPool resource_cache::create_descriptor_pool(std::uint32_t maxSets,
                                                                  std::uint32_t uniformDescriptorCount,
                                                                  std::uint32_t samplerDescriptorCount) {
            vk::DescriptorPoolSize uniformPoolSize(vk::DescriptorType::eUniformBuffer, uniformDescriptorCount);
            vk::DescriptorPoolSize samplerPoolSize(vk::DescriptorType::eCombinedImageSampler, samplerDescriptorCount);

            vk::DescriptorPoolCreateInfo poolCreateInfo(
                vk::DescriptorPoolCreateFlags(),
                maxSets,
                2,
                std::array<vk::DescriptorPoolSize, 2>{uniformPoolSize, samplerPoolSize}.data());

            return m_device.createDescriptorPool(poolCreateInfo);
        }

        descriptor_set resource_cache::create_descriptor_set(vk::DescriptorSetLayout layout) {
            std::vector<vk::DescriptorSetLayout> layouts(settings::m_inFlightFrames, layout);
            const vk::DescriptorSetAllocateInfo allocInfo(
                m_descriptorPools[0], settings::m_inFlightFrames, layouts.data());

            const auto sets = m_device.allocateDescriptorSets(allocInfo);

            descriptor_set descSets;
            std::copy_n(sets.begin(), settings::m_inFlightFrames, descSets.begin());
            return descSets;
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

        const rendering_technique* resource_cache::find_technique(std::string_view techniqueName) const {
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
            return &descriptors->second[static_cast<std::uint32_t>(bindPoint) -
                                        static_cast<std::uint32_t>(base::resource_bind_point::material_static)];
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
            if (m_globalLayout) {
                m_device.destroyDescriptorSetLayout(m_globalLayout);
                m_globalLayout = nullptr;
            }
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
