// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/pipeline.h"

#include <unordered_set>

namespace tst {
namespace engine {
    namespace vulkan {

        class pipeline_cache {
        public:
            pipeline_cache();
            ~pipeline_cache();

            void add_pipeline();

        private:
            std::unordered_set<pipeline> m_pipelines;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
