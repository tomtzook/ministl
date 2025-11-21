#pragma once

#include "concepts.h"

namespace framework {

template<integral t_>
struct atomic_base {
    atomic_base() = default;
    explicit atomic_base(t_ t) : m_t(t) {}
    atomic_base(const atomic_base&) = delete;
    atomic_base(atomic_base&&) = default;
    ~atomic_base() = default;

    atomic_base& operator=(const atomic_base&) = delete;
    atomic_base& operator=(atomic_base&&) = default;

    [[nodiscard]] __attribute__((__always_inline__)) t_ load() const noexcept
    { return __atomic_load_n(&m_t, __ATOMIC_SEQ_CST); }
    [[nodiscard]] __attribute__((__always_inline__)) t_ load() const volatile noexcept
    { return __atomic_load_n(&m_t, __ATOMIC_SEQ_CST); }
    __attribute__((__always_inline__)) void store(t_ t) noexcept
    { __atomic_store_n(&m_t, t, __ATOMIC_SEQ_CST); }
    __attribute__((__always_inline__)) void store(t_ t) volatile noexcept
    { __atomic_store_n(&m_t, t, __ATOMIC_SEQ_CST); }
    [[nodiscard]] __attribute__((__always_inline__))  t_ exchange(t_ t) noexcept
    { return __atomic_exchange_n(&m_t, t, __ATOMIC_SEQ_CST); }
    [[nodiscard]] __attribute__((__always_inline__))  t_ exchange(t_ t) volatile noexcept
    { return __atomic_exchange_n(&m_t, t, __ATOMIC_SEQ_CST); }
    [[nodiscard]] __attribute__((__always_inline__))  bool compare_exchange(t_& t, t_ t2) noexcept
    { return __atomic_compare_exchange_n(&m_t, &t, t2, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); }
    [[nodiscard]] __attribute__((__always_inline__))  bool compare_exchange(t_& t, t_ t2) volatile noexcept
    { return __atomic_compare_exchange_n(&m_t, &t, t2, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); }

private:
    static constexpr int alignment = sizeof(t_) > alignof(t_) ? sizeof(t_) : alignof(t_);
    alignas(alignment) t_ m_t = 0;
};

template<integral t_>
struct atomic;

template<>
struct atomic<bool> {
    using value_type = bool;

    atomic() noexcept = default;
    ~atomic() noexcept = default;
    atomic(const atomic&) noexcept = delete;
    atomic(atomic&&) noexcept = default;

    atomic& operator=(const atomic&) = delete;
    atomic& operator=(atomic&&) = default;

    explicit atomic(const bool b) noexcept : m_base(b) { }

    atomic& operator=(const bool b) noexcept
    { m_base.store(b); return *this; }
    volatile atomic& operator=(const bool b) volatile noexcept
    { m_base.store(b); return *this; }
    [[nodiscard]] explicit operator bool() const noexcept
    { return m_base.load(); }
    [[nodiscard]] explicit operator bool() const volatile noexcept
    { return m_base.load(); }

    [[nodiscard]] bool exchange(const bool b) noexcept
    { return m_base.exchange(b); }
    [[nodiscard]] bool exchange(const bool b) volatile noexcept
    { return m_base.exchange(b); }
    [[nodiscard]] bool compare_exchange(bool& b1, const bool b2) noexcept
    { return m_base.compare_exchange(b1, b2); }
    [[nodiscard]] bool compare_exchange(bool& b1, const bool b2) volatile noexcept
    { return m_base.compare_exchange(b1, b2); }

private:
    atomic_base<bool> m_base;
};

struct atomic_flag {
    atomic_flag() = default;
    atomic_flag(const atomic_flag&) = delete;
    atomic_flag(atomic_flag&&) = default;

    atomic_flag& operator=(const atomic_flag&) = delete;
    atomic_flag& operator=(atomic_flag&&) = default;

    [[nodiscard]] bool test_and_set() {
        bool expected = false;
        return m_flag.compare_exchange(expected, true);
    }

    void clear() { m_flag = false; }

private:
    atomic<bool> m_flag;
};

}
