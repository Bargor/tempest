// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include "resources/shader.h"

#include <map>

namespace tst {
namespace engine {
    namespace vulkan {

        class shader_cache {
        public:
            void add_shaders(std::string& name, shader_set&& shaders);

            std::optional<std::reference_wrapper<shader_set>> find_shaders(const std::string& name);

        private:
            std::map<std::string, shader_set> m_shaders;
        };

    } // namespace vulkan

} // namespace engine
} // namespace tst
