#pragma once

#include "intrinsin.h"
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

    void lock() noexcept;
    void unlock() noexcept;

private:
    atomic_flag m_flag;
};

template<lockable t_>
class unique_lock {
public:
    explicit unique_lock(t_& t) noexcept;
    unique_lock(const unique_lock&) = delete;
    unique_lock(unique_lock&&) noexcept;
    ~unique_lock() noexcept;

    unique_lock& operator=(const unique_lock&) = delete;
    unique_lock& operator=(unique_lock&&) noexcept;

    constexpr explicit operator bool() const noexcept { return m_owns; }
    [[nodiscard]] constexpr bool owns() const noexcept { return m_owns; }

    void lock() noexcept;
    void unlock() noexcept;

private:
    t_& m_lock;
    bool m_owns;
};

template<lockable t_>
unique_lock<t_>::unique_lock(t_& t) noexcept
    : m_lock(t)
    , m_owns(false) {
    lock();
}

template<lockable t_>
unique_lock<t_>::unique_lock(unique_lock&& other) noexcept
    : m_lock(other.m_lock)
    , m_owns(other.m_owns) {
    other.m_owns = false;
}

template<lockable t_>
unique_lock<t_>::~unique_lock() noexcept {
    if (m_owns) {
        unlock();
    }
}

template<lockable t_>
unique_lock<t_>& unique_lock<t_>::operator=(unique_lock&& other) noexcept {
    m_lock = other.m_lock;
    m_owns = other.m_owns;
    other.m_owns = false;
    return *this;
}

template<lockable t_>
void unique_lock<t_>::lock() noexcept {
    m_lock.lock();
    m_owns = true;
}

template<lockable t_>
void unique_lock<t_>::unlock() noexcept {
    m_lock.unlock();
    m_owns = false;
}

}
