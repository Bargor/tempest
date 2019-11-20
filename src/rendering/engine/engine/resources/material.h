// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <engine-impl/api.h>
#include <engine-impl/resources/material.h>

namespace tst {
namespace engine {
    namespace resources {
        class uniform_buffer;

        class material : api::material {
        public:
            using super = api::material;
        private:
        };

        static_assert(!std::is_polymorphic_v<material>);
        static_assert(sizeof(material) == sizeof(api::material));
    } // namespace resources
} // namespace engine
} // namespace tst
