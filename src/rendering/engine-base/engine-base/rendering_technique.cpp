// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "rendering_technique.h"

namespace tst {
namespace engine {
    namespace base {

        rendering_technique::rendering_technique(std::string&& techniqueName)
            : m_techniqueName(std::move(techniqueName)) {
        }
    } // namespace base
} // namespace engine
} // namespace tst
