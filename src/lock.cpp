
#include "lock.h"


namespace framework {

void spin_lock::lock() noexcept {
    while (m_flag.test_and_set(atomic_memory_order::acquire)) {
        _ministl_builtin_pause();
    }
}

void spin_lock::unlock() noexcept {
    m_flag.clear(atomic_memory_order::release);
}

}
