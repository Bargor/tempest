// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"

#include <engine-base/pipeline.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class vertex_format;
        class rendering_technique;
        class settings;

        vk::Pipeline compile_pipeline(const vk::Device logicalDevice,
                                      const base::pipeline& base,
                                      const vertex_format& format,
                                      const shader_set& shaders,
                                      const rendering_technique& technique,
                                      const settings& settings);

        class pipeline : public base::pipeline {
        public:
            friend vk::Pipeline compile_pipeline(const vk::Device logicalDevice,
                                                 const base::pipeline& base,
                                                 const vertex_format& format,
                                                 const shader_set& shaders,
                                                 const rendering_technique& technique,
                                                 const settings& settings);

            using super = base::pipeline;

            pipeline(const vk::Device logicalDevice,
                     base::pipeline&& base,
                     const vertex_format& format,
                     const shader_set& shaders,
                     const rendering_technique& technique,
                     const settings& settings);
            ~pipeline();

            void bind_command_buffer(vk::CommandBuffer& buffer, vk::PipelineBindPoint bindPoint) const;

            const rendering_technique& get_technique() const noexcept;

        private:
            vk::Pipeline m_pipeline;
            const rendering_technique& m_technique;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst

namespace std {

template<>
struct hash<tst::engine::vulkan::pipeline> {
    std::size_t operator()(const tst::engine::vulkan::pipeline& pipeline) const {
        return std::hash<tst::engine::base::pipeline>{}(pipeline);
    }
};

} // namespace std
