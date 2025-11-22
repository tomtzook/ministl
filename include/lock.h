#pragma once

#include <immintrin.h>

#include "concepts.h"
#include "atomic.h"

namespace framework {

template<typename t_>
concept lockable = requires(t_ t) {
    { t.lock() } -> same_as<void>;
    { t.unlock() } -> same_as<void>;
};

class spin_lock {
public:
    constexpr spin_lock() = default;
    spin_lock(const spin_lock&) = delete;
    spin_lock(spin_lock&&) = default;

    spin_lock& operator=(const spin_lock&) = delete;
    spin_lock& operator=(spin_lock&&) = default;

    void lock() noexcept { while (m_flag.test_and_set()) { _mm_pause(); } }
    void unlock() noexcept { m_flag.clear(); }

private:
    atomic_flag m_flag;
};

template<lockable t_>
class unique_lock {
public:
    explicit unique_lock(t_& t) noexcept;
    unique_lock(const unique_lock&) = delete;
    unique_lock(unique_lock&&) = default;
    ~unique_lock() noexcept;

    unique_lock& operator=(const unique_lock&) = delete;
    unique_lock& operator=(unique_lock&&) = default;

private:
    t_& m_lock;
};

template<lockable t_>
unique_lock<t_>::unique_lock(t_& t) noexcept
    : m_lock(t) {
    m_lock.lock();
}

template<lockable t_>
unique_lock<t_>::~unique_lock() noexcept {
    m_lock.unlock();
}

}
