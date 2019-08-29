// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "shader.h"
#include "vertex_format.h"

#include <engine-base/resources/pipeline.h>

namespace tst {
namespace engine {
    namespace vulkan {

        class pipeline : public base::pipeline {
        public:
            pipeline(vertex_format& format, shader_set& shaders);

        private:
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
