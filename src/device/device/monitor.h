// This file is part of Tempest-device project
// Author: Karol Kontny

#pragma once
#include "GLFW/glfw3.h"

#include <common/position.h>
#include <common/rectangle.h>
#include <cstdint>
#include <platform.h>
#include <string>

namespace tst {
namespace device {

    class monitor {
    public:
        monitor() = delete;
        monitor(GLFWmonitor& monitor);

        const std::string& get_name() const noexcept;
        std::uint32_t get_width() const noexcept;
        std::uint32_t get_height() const noexcept;
        std::int32_t get_red_bits() const noexcept;
        std::int32_t get_green_bits() const noexcept;
        std::int32_t get_blue_bits() const noexcept;
        std::int32_t get_refresh_rate() const noexcept;
        std::int32_t get_physical_width() const noexcept;
        std::int32_t get_physical_height() const noexcept;
        core::position<std::int32_t> get_monitor_pos() const noexcept;
        GLFWmonitor* get_handle() const noexcept;

    private:
        std::string m_name;
        GLFWmonitor* m_monitorHandle;
        const GLFWgammaramp* m_gammaRamp;
        const GLFWvidmode* m_videoMode;
        std::uint32_t m_width;
        std::uint32_t m_height;
        std::int32_t m_redBits;
        std::int32_t m_greenBits;
        std::int32_t m_blueBits;
        std::int32_t m_refreshRate;
        core::extent<std::int32_t> m_physicalSize;
        core::position<std::int32_t> m_position;
    };

    TST_INLINE const std::string& monitor::get_name() const noexcept {
        return m_name;
    }

    TST_INLINE std::uint32_t monitor::get_width() const noexcept {
        return m_width;
    }

    TST_INLINE std::uint32_t monitor::get_height() const noexcept {
        return m_height;
    }

    TST_INLINE std::int32_t monitor::get_red_bits() const noexcept {
        return m_redBits;
    }

    TST_INLINE std::int32_t monitor::get_green_bits() const noexcept {
        return m_greenBits;
    }

    TST_INLINE std::int32_t monitor::get_blue_bits() const noexcept {
        return m_blueBits;
    }

    TST_INLINE std::int32_t monitor::get_refresh_rate() const noexcept {
        return m_refreshRate;
    }

    TST_INLINE std::int32_t monitor::get_physical_width() const noexcept {
        return m_physicalSize.width;
    }

    TST_INLINE std::int32_t monitor::get_physical_height() const noexcept {
        return m_physicalSize.height;
    }

    TST_INLINE core::position<std::int32_t> monitor::get_monitor_pos() const noexcept {
        return m_position;
    }

    TST_INLINE GLFWmonitor* monitor::get_handle() const noexcept {
        return m_monitorHandle;
    }

} // namespace device
} // namespace tst
