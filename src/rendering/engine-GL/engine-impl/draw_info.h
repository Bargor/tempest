// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "engine-base/view.h"
#include "resources/pipeline.h"

namespace tst {
namespace engine {
    namespace opengl {

        class material;
        class index_buffer;
        class vertex_buffer;
        class uniform_buffer;

        class draw_info {
        public:
            draw_info(const vertex_buffer& vertices,
                      const index_buffer* indices,
                      pipeline::pipeline_hash pipeline,
                      const base::view& view,
                      const material& material,
                      const std::vector<const uniform_buffer*>& uniforms);

            ~draw_info() = default;

        public:
            const vertex_buffer& vertices;
            const index_buffer* indices;
            pipeline::pipeline_hash pipelineHash;
            const base::view viewData;
            const material& meshMaterial;
            const std::vector<const uniform_buffer*>& uniforms;
        };
    } // namespace opengl
} // namespace engine
} // namespace tst