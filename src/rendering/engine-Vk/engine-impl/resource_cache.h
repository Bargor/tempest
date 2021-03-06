// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/pipeline.h"
#include "resources/rendering_technique.h"
#include "resources/settings.h"
#include "resources/shader.h"

#include <engine-base/resource_bind_point.h>
#include <unordered_map>
#include <unordered_set>

namespace tst {
namespace engine {
    namespace vulkan {

        class swap_chain;
        using descriptor_set = std::array<vk::DescriptorSet, settings::m_inFlightFrames>;

        class resource_cache {
        public:
            resource_cache(const vk::Device device);
            resource_cache(const resource_cache&) = delete;

            ~resource_cache();

            std::size_t add_pipeline(pipeline&& newPipeline);
            void add_rendering_technique(std::string&& techniqueName,
                                         base::technique_settings&& settings,
                                         vk::Device device,
                                         const swap_chain& swapChain);
            void add_shaders(std::string name, shader_set&& shaders);

            const pipeline* find_pipeline(std::size_t pipelineHash) const;
            const rendering_technique* find_technique(std::string_view name) const;
            const shader_set* find_shaders(const std::string& name) const;
            const std::vector<vk::DescriptorSetLayout>* find_descriptor_layouts(const std::string& shadersName) const noexcept;
            const descriptor_set* find_descriptor_sets(const std::string& shadersName,
                                                       base::resource_bind_point bindPoint) const;
            vk::DescriptorSetLayout get_global_layout() const noexcept;
            const descriptor_set& get_global_static_set() const noexcept;
            const descriptor_set& get_global_dynamic_set() const noexcept;
            const descriptor_set& get_view_static_set() const noexcept;
            const descriptor_set& get_view_dynamic_set() const noexcept;

            vk::DescriptorPool get_gui_descritptor_pool() const;

            void clear();
            void destroy();
            void rebuild_techniques(const swap_chain& newSwapChain);
            void rebuild_pipelines();

        private:
            vk::DescriptorPool create_descriptor_pool(std::uint32_t maxSets,
                                                      std::uint32_t uniformDescriptorCount,
                                                      std::uint32_t samplerDescriptorCount);
            descriptor_set create_descriptor_set(vk::DescriptorSetLayout layout);

        private:
            vk::Device m_device;
            std::vector<vk::DescriptorPool> m_descriptorPools;
            std::unordered_map<std::size_t, pipeline> m_pipelines;
            std::vector<rendering_technique> m_techniques;
            std::unordered_map<std::string, shader_set> m_shaders;
            std::unordered_map<std::string, std::vector<descriptor_set>> m_descriptorSets;
            vk::DescriptorSetLayout m_globalLayout;
            descriptor_set m_globalStaticSet;
            descriptor_set m_globalDynamicSet;
            descriptor_set m_viewStaticSet;
            descriptor_set m_viewDynamicSet;
        };

        TST_INLINE vk::DescriptorSetLayout resource_cache::get_global_layout() const noexcept {
            return m_globalLayout;
        }

        TST_INLINE const descriptor_set& resource_cache::get_global_static_set() const noexcept {
            return m_globalStaticSet;
        }

        TST_INLINE const descriptor_set& resource_cache::get_global_dynamic_set() const noexcept {
            return m_globalDynamicSet;
        }

        TST_INLINE const descriptor_set& resource_cache::get_view_static_set() const noexcept {
            return m_viewStaticSet;
        }

        TST_INLINE const descriptor_set& resource_cache::get_view_dynamic_set() const noexcept {
            return m_viewDynamicSet;
        }

        TST_INLINE vk::DescriptorPool resource_cache::get_gui_descritptor_pool() const {
            return m_descriptorPools[1];
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
