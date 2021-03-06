// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "resources/pipeline.h"

#include <engine-base/resource_bind_point.h>
#include <engine-base/view.h>
#include <util/flags.h>
#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {

        class material;
        class index_buffer;
        class vertex_buffer;
        class uniform_buffer;

        struct draw_info {
        public:
            draw_info(const vertex_buffer& vertices,
                      const index_buffer* indices,
                      pipeline::pipeline_hash pipeline,
                      const base::view& view,
                      const material& material,
                      const std::vector<const uniform_buffer*>& uniforms);

            draw_info(const draw_info& other) = default;
            draw_info(draw_info&& other) noexcept;

            ~draw_info() = default;

        public:
            const vertex_buffer& vertices;
            const index_buffer* indices;
            const pipeline::pipeline_hash pipelineHash;
            const base::view viewData;
            const material& meshMaterial;
            std::vector<vk::DescriptorSet> descriptorSets;
            const pipeline* pipelineState;
            base::descritptor_bind_flags descriptorBindFlags;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
