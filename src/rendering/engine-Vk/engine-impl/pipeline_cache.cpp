// This file is part of Tempest-engine project
// Author: Karol Kontny
#include "pipeline_cache.h"

namespace tst {
namespace engine {
    namespace vulkan {

        pipeline_cache::pipeline_cache(vk::Device& device) : m_device(device) {
        }

        pipeline_cache::~pipeline_cache() {
        }

        void pipeline_cache::add_pipeline(pipeline& pipeline) {
            m_pipelines.insert(pipeline);
        }
    } // namespace vulkan
} // namespace engine
} // namespace tst
