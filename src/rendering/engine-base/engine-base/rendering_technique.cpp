// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "rendering_technique.h"

namespace tst {
namespace engine {
    namespace base {

        rendering_technique::rendering_technique(const std::string& techniqueName, technique_settings&& settings)
            : m_techniqueName(techniqueName), m_settings(std::move(settings)) {
        }
    } // namespace base
} // namespace engine
} // namespace tst
