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
            pipeline_cache(vk::Device& device);
            pipeline_cache(const pipeline_cache&) = delete;
            ~pipeline_cache();

            void add_pipeline(pipeline& pipeline);

        private:
            vk::Device& m_device;

            std::unordered_set<pipeline> m_pipelines;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst
