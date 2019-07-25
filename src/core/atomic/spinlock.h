// This file is part of Tempest-core project
// Author: Karol Kontny

#include <atomic>
#include <core.h>

namespace tst {
namespace core {

    class spinlock {
    public:
        void lock() noexcept;

        void unlock() noexcept;

    private:
        std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
    };

    void TST_INLINE spinlock::lock() noexcept {
        while (m_lock.test_and_set()) {
        }
    }

    void TST_INLINE spinlock::unlock() noexcept {
        m_lock.clear();
    }

} // namespace core
} // namespace tst