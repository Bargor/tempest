// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"

#include <engine-base/pipeline_settings.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_format;
        class rendering_technique;
        class settings;

        class pipeline {
            friend std::hash<tst::engine::vulkan::pipeline>;

        public:
            pipeline(const vk::Device logicalDevice,
                     const base::pipeline_settings& pipelineSettings,
                     const vertex_format& format,
                     const shader_set& shaders,
                     const rendering_technique& technique);
            ~pipeline();

            void bind_command_buffer(vk::CommandBuffer& buffer, vk::PipelineBindPoint bindPoint) const;

            const rendering_technique& get_technique() const noexcept;

        private:
            vk::Pipeline m_pipeline;
            const base::pipeline_settings& m_pipelineSettings;
            const rendering_technique& m_technique;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst

namespace std {

template<>
struct hash<tst::engine::vulkan::pipeline> {
    std::size_t operator()(const tst::engine::vulkan::pipeline& pipeline) const {
        return std::hash<tst::engine::base::pipeline_settings>{}(pipeline.m_pipelineSettings);
    }
};

} // namespace std
