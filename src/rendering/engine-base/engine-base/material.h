// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <string>

namespace tst {
namespace engine {
    namespace base {
        class material {
        public:
            material(const std::string& name);
        private:
            std::string m_materialName;
        };
    } // namespace base
} // namespace engine
} // namespace tst
