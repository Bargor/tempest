// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "shader_cache.h"

namespace tst {
namespace engine {
    namespace vulkan {

        void shader_cache::add_shaders(std::string& name, shader_set&& shaders) {
            m_shaders.insert(std::make_pair(name, std::move(shaders)));
        }

        std::optional<std::reference_wrapper<shader_set>> shader_cache::find_shaders(const std::string& name) {
            auto shaders = m_shaders.find(name);
            if (shaders == m_shaders.end()) {
                return std::ref(shaders->second);
            }
            return std::nullopt;
        }

    } // namespace vulkan
} // namespace engine
} // namespace tst
