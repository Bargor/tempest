// This file is part of Tempest-engine project
// Author: Karol Kontny
#pragma once

#include <string>

namespace tst {
namespace engine {
    namespace base {

        class rendering_technique {
        public:
            rendering_technique(const std::string& techniqueName);

        private:
            std::string m_techniqueName;
        };
    } // namespace base
} // namespace engine
} // namespace tst
