#pragma once

#include "concepts.h"
#include "intrinsin.h"

namespace framework {

enum class atomic_memory_order : int {
    relaxed = _ministl_builtin_atomic_order_relaxed,
    consume = _ministl_builtin_atomic_order_consume,
    acquire = _ministl_builtin_atomic_order_acquire,
    release = _ministl_builtin_atomic_order_release,
    acq_rel = _ministl_builtin_atomic_order_acq_rel,
    seq_cst = _ministl_builtin_atomic_order_seq_cst
};

template<integral t_>
struct atomic_storage {
    using type = t_;

    atomic_storage() = default;
    explicit atomic_storage(type t) : m_t(t) {}
    atomic_storage(const atomic_storage&) = default;
    atomic_storage(atomic_storage&&) = default;
    ~atomic_storage() = default;

    atomic_storage& operator=(const atomic_storage&) = default;
    atomic_storage& operator=(atomic_storage&&) = default;

    [[nodiscard]] __attribute__((__always_inline__))
    type load(const atomic_memory_order order) const noexcept
    { return _ministl_builtin_atomic_load(&m_t, static_cast<int>(order)); }
    [[nodiscard]] __attribute__((__always_inline__))
    type load(const atomic_memory_order order) const volatile noexcept
    { return _ministl_builtin_atomic_load(&m_t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    void store(type t, const atomic_memory_order order) noexcept
    { _ministl_builtin_atomic_store(&m_t, t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    void store(type t, const atomic_memory_order order) volatile noexcept
    { _ministl_builtin_atomic_store(&m_t, t, static_cast<int>(order)); }

    [[nodiscard]] __attribute__((__always_inline__))
    type exchange(type t, const atomic_memory_order order) noexcept
    { return _ministl_builtin_atomic_exchange(&m_t, t, static_cast<int>(order)); }
    [[nodiscard]] __attribute__((__always_inline__))
    type exchange(type t, const atomic_memory_order order) volatile noexcept
    { return _ministl_builtin_atomic_exchange(&m_t, t, static_cast<int>(order)); }
    [[nodiscard]] __attribute__((__always_inline__))
    bool compare_exchange(type& expect, type desired, const atomic_memory_order order) noexcept
    { return _ministl_builtin_atomic_cmpxchg(&m_t, &expect, desired, static_cast<int>(order), static_cast<int>(order)); }
    [[nodiscard]] __attribute__((__always_inline__))
    bool compare_exchange(type& expect, type desired, const atomic_memory_order order) volatile noexcept
    { return _ministl_builtin_atomic_cmpxchg(&m_t, &expect, desired, static_cast<int>(order), static_cast<int>(order)); }

    __attribute__((__always_inline__))
    type fetch_add(type t, const atomic_memory_order order) noexcept
    { return _ministl_builtin_atomic_fetch_add(&m_t, t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    type fetch_add(type t, const atomic_memory_order order) volatile noexcept
    { return _ministl_builtin_atomic_fetch_add(&m_t, t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    type add_fetch(type t, const atomic_memory_order order) noexcept
    { return _ministl_builtin_atomic_add_fetch(&m_t, t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    type add_fetch(type t, const atomic_memory_order order) volatile noexcept
    { return _ministl_builtin_atomic_add_fetch(&m_t, t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    type fetch_sub(type t, const atomic_memory_order order) noexcept
    { return _ministl_builtin_atomic_fetch_sub(&m_t, t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    type fetch_sub(type t, const atomic_memory_order order) volatile noexcept
    { return _ministl_builtin_atomic_fetch_sub(&m_t, t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    type sub_fetch(type t, const atomic_memory_order order) noexcept
    { return _ministl_builtin_atomic_sub_fetch(&m_t, t, static_cast<int>(order)); }
    __attribute__((__always_inline__))
    type sub_fetch(type t, const atomic_memory_order order) volatile noexcept
    { return _ministl_builtin_atomic_sub_fetch(&m_t, t, static_cast<int>(order)); }

private:
    static constexpr int alignment = sizeof(t_) > alignof(t_) ? sizeof(t_) : alignof(t_);
    alignas(alignment) t_ m_t = 0;
};

template<integral t_>
struct atomic_base {
    using type = t_;

    atomic_base() noexcept = default;
    ~atomic_base() noexcept = default;
    atomic_base(const atomic_base&) noexcept = default;
    atomic_base(atomic_base&&) noexcept = default;
    explicit atomic_base(const type t) noexcept : m_base(t) { }

    atomic_base& operator=(const atomic_base&) = default;
    atomic_base& operator=(atomic_base&&) = default;

    [[nodiscard]] type load(const atomic_memory_order order = atomic_memory_order::seq_cst) const noexcept
    { return m_base.load(order); }
    [[nodiscard]] type load(const atomic_memory_order order = atomic_memory_order::seq_cst) const volatile noexcept
    { return m_base.load(order); }
    void store(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) noexcept
    { return m_base.store(t, order); }
    void store(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept
    { return m_base.store(t, order); }

    [[nodiscard]] type exchange(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) noexcept
    { return m_base.exchange(t, order); }
    [[nodiscard]] type exchange(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept
    { return m_base.exchange(t, order); }
    [[nodiscard]] bool compare_exchange(type& expect, const type desired, const atomic_memory_order order = atomic_memory_order::seq_cst) noexcept
    { return m_base.compare_exchange(expect, desired, order); }
    [[nodiscard]] bool compare_exchange(type& expect, const type desired, const atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept
    { return m_base.compare_exchange(expect, desired, order); }

protected:
    atomic_storage<type> m_base;
};

template<typename t_>
struct atomic;

template<>
struct atomic<bool> final : atomic_base<bool> {
    using type = bool;

    atomic() noexcept = default;
    ~atomic() noexcept = default;
    atomic(const atomic&) noexcept = default;
    atomic(atomic&&) noexcept = default;
    explicit atomic(const type t) noexcept : atomic_base(t) { }

    atomic& operator=(const atomic&) = default;
    atomic& operator=(atomic&&) = default;

    [[nodiscard]] explicit operator type() const noexcept { return load(); }
    [[nodiscard]] explicit operator type() const volatile noexcept { return load(); }
    atomic& operator=(const type t) noexcept { store(t); return *this; }
    volatile atomic& operator=(const type t) volatile noexcept { store(t); return *this; }
};

template<integral t_>
struct atomic<t_> final : atomic_base<t_> {
    using type = t_;

    atomic() noexcept = default;
    ~atomic() noexcept = default;
    atomic(const atomic&) noexcept = default;
    atomic(atomic&&) noexcept = default;
    explicit atomic(const type t) noexcept : atomic_base<t_>(t) { }

    atomic& operator=(const atomic&) = default;
    atomic& operator=(atomic&&) = default;

    [[nodiscard]] explicit operator type() const noexcept { return this->load();  }
    [[nodiscard]] explicit operator type() const volatile noexcept { return this->load(); }
    atomic& operator=(const type t) noexcept { this->store(t); return *this; }
    volatile atomic& operator=(const type t) volatile noexcept { this->store(t); return *this; }

    type fetch_add(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) noexcept { return this->m_base.fetch_add(t, order); }
    type fetch_add(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept { return this->m_base.fetch_add(t, order); }
    type add_fetch(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) noexcept { return this->m_base.add_fetch(t, order); }
    type add_fetch(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept { return this->m_base.add_fetch(t, order); }
    type fetch_sub(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) noexcept { return this->m_base.fetch_sub(t, order); }
    type fetch_sub(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept { return this->m_base.fetch_sub(t, order); }
    type sub_fetch(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) noexcept { return this->m_base.sub_fetch(t, order); }
    type sub_fetch(const type t, const atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept { return this->m_base.sub_fetch(t, order); }

    type operator++(int) noexcept { return fetch_add(1); }
    type operator++(int) volatile noexcept { return fetch_add(1); }
    type operator++() noexcept { return add_fetch(1); }
    type operator++() volatile noexcept { return add_fetch(1); }
    atomic& operator+=(const type other) noexcept { add_fetch(other); return *this; }
    atomic& operator+=(const type other) volatile noexcept { add_fetch(other); return *this; }

    type operator--(int) noexcept { return fetch_sub(1); }
    type operator--(int) volatile noexcept { return fetch_sub(1); }
    type operator--() noexcept { return sub_fetch(1); }
    type operator--() volatile noexcept { return sub_fetch(1); }
    atomic& operator-=(const type other) noexcept { sub_fetch(other); return *this; }
    atomic& operator-=(const type other) volatile noexcept { sub_fetch(other); return *this; }
};

struct atomic_flag {
    atomic_flag() = default;
    atomic_flag(const atomic_flag&) = delete;
    atomic_flag(atomic_flag&&) = default;

    atomic_flag& operator=(const atomic_flag&) = delete;
    atomic_flag& operator=(atomic_flag&&) = default;

    [[nodiscard]] bool test_and_set(atomic_memory_order order = atomic_memory_order::seq_cst) noexcept;
    [[nodiscard]] bool test_and_set(atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept;
    void clear(atomic_memory_order order = atomic_memory_order::seq_cst) noexcept;
    void clear(atomic_memory_order order = atomic_memory_order::seq_cst) volatile noexcept;

private:
    atomic<bool> m_flag;
};

}
