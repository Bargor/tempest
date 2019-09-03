// This file is part of Tempest project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

#include <vulkan/vulkan.hpp>

namespace tst {
namespace engine {
    namespace vulkan {
        struct draw_info {
        public:
            draw_info(const uniform_buffer_object& transformation,
                      const vertex_buffer* vertices,
                      const index_buffer<std::uint16_t>* indices,
                      std::size_t pipelineHash);

            ~draw_info() = default;

        public:
            const uniform_buffer_object& transformation;
            const vertex_buffer* vertices;
            const index_buffer<std::uint16_t>* indices;
            std::size_t pipelineHash;
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
