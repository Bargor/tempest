// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/texture.h"

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {
        struct draw_info {
        public:
            draw_info(const vertex_buffer* vertices,
                      const index_buffer<std::uint16_t>* indices,
                      const pipeline& pipeline,
                      const std::vector<const uniform_buffer*>& uniforms,
                      const std::vector<const texture*>& textures);

            draw_info(draw_info&& other) noexcept;

            ~draw_info() = default;

        public:
            const vertex_buffer* vertices;
            const index_buffer<std::uint16_t>* indices;
            const pipeline& pipelineState;
            std::vector<vk::DescriptorSet> descriptorSets;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
