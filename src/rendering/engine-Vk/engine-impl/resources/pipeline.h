// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"
#include "vertex_format.h"
#include "rendering_technique.h"

#include <engine-base/pipeline_settings.h>

namespace tst {
namespace engine {
    namespace base {
        struct draw_settings;
    }

    namespace vulkan {

        struct settings;

        class pipeline {
            friend std::hash<tst::engine::vulkan::pipeline>;

        public:
            using pipeline_hash = std::size_t;

        public:
            pipeline(const vk::Device logicalDevice,
                     const settings& engineSettings,
                     base::draw_settings&& drawSettings,
                     const vertex_format& format,
                     const shader_set& shaders,
                     const rendering_technique& technique,
                     std::vector<vk::DescriptorSetLayout>&& layouts,
                     vk::Extent2D extent);
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
            base::viewport_callback m_viewportSettingsCallback;
            base::scissor_callback m_scissorCallback;
            base::pipeline_settings m_pipelineSettings;
            vk::PipelineLayout m_pipelineLayout;
            vk::Pipeline m_pipeline;
            const rendering_technique& m_technique;
            const shader_set& m_shaders;
            const vertex_format m_vertexFormat;
            const vk::Device m_logicalDevice;
            std::vector<vk::DescriptorSetLayout> m_layouts;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst

namespace std {

template<>
struct hash<tst::engine::vulkan::pipeline> {
    std::size_t operator()(const tst::engine::vulkan::pipeline& pipeline) const {
        std::size_t seed = 0;
        hash<std::string_view> hasher;
        tst::hash_combine(seed, hasher(pipeline.m_technique.get_name()));
        tst::hash_combine(seed, std::hash<tst::engine::base::pipeline_settings>{}(pipeline.m_pipelineSettings));
        return seed;
    }
};

} // namespace std
