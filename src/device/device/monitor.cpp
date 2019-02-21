// This file is part of Tempest-application project
// Author: Karol Kontny

#include "monitor.h"

#include "assert.h"

#include <thread/main_thread.h>

namespace tst {
namespace device {

    monitor::monitor(GLFWmonitor& monitor)
        : m_name(glfwGetMonitorName(&monitor))
        , m_monitorHandle(&monitor)
        , m_gammaRamp(glfwGetGammaRamp(&monitor))
        , m_videoMode(glfwGetVideoMode(&monitor))
        , m_width(m_videoMode->width)
        , m_height(m_videoMode->height)
        , m_redBits(m_videoMode->redBits)
        , m_greenBits(m_videoMode->greenBits)
        , m_blueBits(m_videoMode->blueBits)
        , m_refreshRate(m_videoMode->refreshRate) {
        assert(std::this_thread::get_id() == core::main_thread::get_id());
        glfwGetMonitorPhysicalSize(&monitor, &m_physicalWidth, &m_physicalWidth);
        glfwGetMonitorPos(&monitor, &m_position.x, &m_position.y);
    }

} // namespace device
} // namespace tst