// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"

namespace tst {
namespace engine {
    namespace opengl {
        class draw_info {
        public:
            draw_info(const vertex_buffer* vertices, const index_buffer<std::uint16_t>* indices, const pipeline& pipeline, const uniform_buffer* uniforms);

            ~draw_info() = default;

        public:
            const vertex_buffer* vertices;
            const index_buffer<std::uint16_t>* indices;
            const pipeline& pipelineState;
            const uniform_buffer* uniforms;
        };
    } // namespace opengl
} // namespace engine
} // namespace tst