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

        class pipeline : public base::pipeline {
        public:
            using super = base::pipeline;

            pipeline(const vk::Device logicalDevice,
                     base::pipeline&& base,
                     const vertex_format& format,
                     const shader_set& shaders,
                     const rendering_technique& technique);
            ~pipeline();

            void bind_command_buffer(vk::CommandBuffer& buffer, vk::PipelineBindPoint& bindPoint);

        private:
            vk::Pipeline m_pipeline;
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
