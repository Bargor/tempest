// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/pipeline.h"
#include "resources/rendering_technique.h"
#include "resources/shader.h"

#include <unordered_map>
#include <unordered_set>

namespace tst {
namespace engine {
    namespace vulkan {

        class resource_cache {
        public:
            void add_pipeline(pipeline&& newPipeline);
            void add_rendering_technique(rendering_technique&& technique);
            void add_shaders(const std::string& name, shader_set&& shaders);

            const pipeline* find_pipeline() const;
            const rendering_technique* find_technique(const std::string& name) const;
            const shader_set* find_shaders(const std::string& name) const;

        private:
            std::unordered_map<std::size_t, pipeline> m_pipelines;
            std::vector<rendering_technique> m_techniques;
            std::unordered_map<std::string, shader_set> m_shaders;
        };

    } // namespace vulkan
} // namespace engine
} // namespace tst