// This file is part of Tempest-engine project
// Author: Karol Kontny

#include "rendering_technique.h"

namespace tst {
namespace engine {
    namespace base {
        rendering_technique::rendering_technique(const std::string& techniqueName,
                                                 const viewport_settings& viewport,
                                                 const core::rectangle<std::int32_t, std::uint32_t> scissor,
                                                 std::initializer_list<color_blending_settings> framebufferBlending,
                                                 const global_blending_settings& globalBlending)
            : m_techniqueName(techniqueName)
            , m_viewportSettings(viewport)
            , m_scissor(scissor)
            , m_framebufferColorBlending(framebufferBlending)
            , m_globalColorBlending(globalBlending) {
        }
    } // namespace base
} // namespace engine
} // namespace tst
