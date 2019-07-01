// This file is part of Tempest-core project
// Author: Karol Kontny

#pragma once

#include <assert.h>
#include <chrono>
#include <core.h>
#include <cstdint>
#include <platform.h>
#include <ratio>

namespace tst {
namespace core {

    using hr_clock = std::chrono::high_resolution_clock;
    using sys_clock = std::chrono::system_clock;
    
    using time_point = std::chrono::time_point<hr_clock, std::chrono::microseconds>;

    class timer {
    public:
		using clock_type = hr_clock;
    public:
        template<typename U, typename T>
        using dur = std::chrono::duration<U, T>;

    public:
        timer() noexcept;
        ~timer() = default;

        void start() noexcept;
        void stop() noexcept;
        void reset() noexcept;
        std::chrono::microseconds get_time() const noexcept;
        bool started() const noexcept;
        time_point now() const noexcept;

    private:
        hr_clock m_timer;
        std::chrono::time_point<hr_clock> m_lastStart;
        dur<std::int64_t, std::nano> m_elapsedTime;
        bool m_started;
    };

    TST_INLINE tst::core::timer::timer() noexcept : m_elapsedTime(std::chrono::nanoseconds::zero()), m_started(false) {
    }

    TST_INLINE void timer::start() noexcept {
        m_started = true;
        m_lastStart = m_timer.now();
    }

    TST_INLINE void timer::stop() noexcept {
        assert(m_started);
        m_elapsedTime += (m_timer.now() - m_lastStart);
        m_started = false;
    }

    TST_INLINE void timer::reset() noexcept {
        m_started = false;
        m_elapsedTime = std::chrono::nanoseconds::zero();
    }

    TST_INLINE std::chrono::microseconds timer::get_time() const noexcept {
        return m_started ? std::chrono::duration_cast<std::chrono::microseconds>(m_timer.now() - m_lastStart) :
                           std::chrono::duration_cast<std::chrono::microseconds>(m_elapsedTime);
    }

    TST_INLINE bool timer::started() const noexcept {
        return m_started;
    }

	TST_INLINE time_point timer::now() const noexcept {
        return m_started ? std::chrono::time_point_cast<std::chrono::microseconds>(m_timer.now()) :
                           std::chrono::time_point_cast<std::chrono::microseconds>(m_lastStart + m_elapsedTime);
	}

} // namespace core
} // namespace tst
