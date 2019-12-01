// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"
#include "vertex_format.h"

#include <engine-base/pipeline_settings.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class rendering_technique;
        class settings;

        class pipeline {
            friend std::hash<tst::engine::vulkan::pipeline>;

        public:
            pipeline(const vk::Device logicalDevice,
                     const settings& engineSettings,
                     const vertex_format& format,
                     const shader_set& shaders,
                     const rendering_technique& technique);
            pipeline(const pipeline&) = delete;
            pipeline(pipeline&& pipeline) noexcept;

            ~pipeline();

            void bind_command_buffer(vk::CommandBuffer buffer, vk::PipelineBindPoint bindPoint) const;

            const rendering_technique& get_technique() const noexcept;
            vk::PipelineLayout get_layout() const noexcept;

            void recreate();

        private:
            void destroy();
        private:
            vk::PipelineLayout m_pipelineLayout;
            vk::Pipeline m_pipeline;
            base::pipeline_settings m_pipelineSettings;
            const rendering_technique& m_technique;
            const shader_set& m_shaders;
            const vertex_format m_vertexFormat;
            const vk::Device m_logicalDevice;
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
