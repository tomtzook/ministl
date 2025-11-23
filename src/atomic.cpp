
#include "atomic.h"

namespace framework {

bool atomic_flag::test_and_set(const atomic_memory_order order) noexcept {
    bool expected = false;
    return m_flag.compare_exchange(expected, true, order);
}

bool atomic_flag::test_and_set(const atomic_memory_order order) volatile noexcept {
    bool expected = false;
    return m_flag.compare_exchange(expected, true, order);
}

void atomic_flag::clear(const atomic_memory_order order) noexcept {
    m_flag.store(false, order);
}

void atomic_flag::clear(const atomic_memory_order order) volatile noexcept {
    m_flag.store(false, order);
}

}
