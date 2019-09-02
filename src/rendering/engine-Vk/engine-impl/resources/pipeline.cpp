// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "pipeline.h"

namespace tst {
namespace engine {
    namespace vulkan {

        pipeline::pipeline(base::pipeline&& base, const vertex_format& format, const shader_set& shaders)
            : super(std::move(base)) {
        }

        pipeline::~pipeline() {
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
