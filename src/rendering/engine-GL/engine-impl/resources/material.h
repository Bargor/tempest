// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-base/material.h>

#include <vector>

namespace tst {
namespace engine {
    namespace opengl {
        class material : base::material {
        public:
            struct creation_info {
                const std::vector<std::string>& textureNames;
                std::uint32_t staticStorageSize;
                std::uint32_t dynamicStorageSize;
            };
        public:
            using super = base::material;

            material(std::string&& name,
                     const std::string& shaderName,
                     const creation_info& info);

        private:
        };
    } // namespace vulkan
} // namespace engine
} // namespace tst
