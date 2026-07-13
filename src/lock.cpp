
#include "lock.h"


namespace framework {

void spin_lock::lock() noexcept {
    while (true) {
        if (m_flag.test_and_set(atomic_memory_order::acquire)) {
            return;
        }

        while (m_flag.test(atomic_memory_order::relaxed)) {
            _ministl_builtin_pause();
        }
    }
}

void spin_lock::unlock() noexcept {
    m_flag.clear(atomic_memory_order::release);
}

}
