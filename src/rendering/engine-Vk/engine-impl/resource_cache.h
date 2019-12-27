// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/pipeline.h"
#include "resources/rendering_technique.h"
#include "resources/settings.h"
#include "resources/shader.h"

#include <unordered_map>
#include <unordered_set>

namespace tst {
namespace engine {
    namespace vulkan {

        class swap_chain;

        class resource_cache {
        public:
            //resource_cache(const resource_cache&) = delete;

            std::size_t add_pipeline(pipeline&& newPipeline);
            void add_rendering_technique(rendering_technique&& technique);
            void add_shaders(const std::string& name, shader_set&& shaders);
            void add_descritptor_set_layout(shader::descriptor_layout&& layout, vk::DescriptorSetLayout vkLayout);

            const pipeline* find_pipeline(std::size_t pipelineHash) const;
            const rendering_technique* find_technique(const std::string& name) const;
            const shader_set* find_shaders(const std::string& name) const;
            const vk::DescriptorSetLayout* find_descriptor_layout(const shader::descriptor_layout& layout);

            void clear();
            void rebuild_techniques(const swap_chain& newSwapChain);
            void rebuild_pipelines();

        private:
            std::unordered_map<std::size_t, pipeline> m_pipelines;
            std::vector<rendering_technique> m_techniques;
            std::unordered_map<std::string, shader_set> m_shaders;
            std::array<vk::DescriptorPool, settings::m_inFlightFrames> m_descriptorPools;
            std::unordered_map<shader::descriptor_layout, vk::DescriptorSetLayout> m_descriptorLayouts;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
