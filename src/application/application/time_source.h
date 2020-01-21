// This file is part of Tempest project
// Author: Karol Kontny

#pragma once

#include <time/timer.h>

namespace tst {
namespace application {
	
	class time_source {
    public:
        time_source() noexcept;

		core::time_point now() const noexcept;
        std::chrono::microseconds get_time() const noexcept;
    private:
		core::timer m_timer;
	};

	TST_INLINE time_source::time_source() noexcept : m_timer(core::timer()) {
        m_timer.start();
    }

    TST_INLINE std::chrono::microseconds time_source::get_time() const noexcept {
        return m_timer.get_time();
    }

    TST_INLINE core::time_point time_source::now() const noexcept {
        return m_timer.now();
	}

}
} // namespace tst