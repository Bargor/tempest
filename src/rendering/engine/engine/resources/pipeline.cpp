// This file is part of Tempest project
// Author: Karol Kontny

#include "pipeline.h"

namespace tst {
namespace engine {
    namespace resources {

        pipeline::pipeline(api::pipeline&& pipelineImpl) : api::pipeline(std::move(pipelineImpl)) {
        }

        pipeline::~pipeline() {
        }

    } // namespace resources
} // namespace engine
} // namespace tst
