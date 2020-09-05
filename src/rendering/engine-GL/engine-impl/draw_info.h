// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/index_buffer.h"
#include "resources/pipeline.h"
#include "resources/uniform_buffer.h"
#include "resources/vertex_buffer.h"
#include "resources/material.h"

namespace tst {
namespace engine {
    namespace opengl {
        class draw_info {
        public:
            draw_info(const vertex_buffer& vertices,
                      const index_buffer* indices,
                      pipeline::pipeline_hash pipeline,
                      const material& material,
                      const std::vector<const uniform_buffer*>& uniforms);

            ~draw_info() = default;

        public:
            const vertex_buffer& vertices;
            const index_buffer* indices;
            pipeline::pipeline_hash pipeline;
            const material& meshMaterial;
            const std::vector<const uniform_buffer*>& uniforms;
        };
    } // namespace opengl
} // namespace engine
} // namespace tst