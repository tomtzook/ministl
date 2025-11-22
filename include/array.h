#pragma once

#include "intrinsin.h"
#include "types.h"
#include "span.h"
#include "terminate.h"

namespace framework {

template<typename t_, size_t size_>
struct base_array {
    using type = t_[size_];
};

template<typename t_>
struct base_array<t_, 0> {
    // empty type used instead of t_[0]
    struct type {
        // indexing is undefined.
        __attribute__((__always_inline__,__noreturn__))
        t_& operator[](size_t) const noexcept { catastrophic_error("array of 0 size access"); }

        // conversion to a pointer produces a null pointer.
        __attribute__((__always_inline__))
        constexpr explicit operator t_*() const noexcept { return nullptr; }
    };
};

template<typename t_, size_t size_>
class array {
public:
    using type = t_;

    using const_pointer = const type*;
    using const_reference = const type&;
    using const_span = span<const type>;
    using pointer = type*;
    using reference = type&;
    using span = span<type>;

    static constexpr size_t type_size = sizeof(type);

    constexpr array() noexcept = default;
    constexpr array(const array&) noexcept = default;
    constexpr array(array&&) noexcept = default;

    constexpr array& operator=(const array&) noexcept = default;
    constexpr array& operator=(array&&) noexcept = default;

    [[nodiscard]] constexpr const_pointer data(const size_t offset=0) const noexcept { if constexpr (offset >= size_) { catastrophic_error("out of array size"); } return m_base + offset; }
    [[nodiscard]] constexpr pointer data(const size_t offset=0) noexcept { if constexpr (offset >= size_) { catastrophic_error("out of array size"); } return m_base + offset; }
    [[nodiscard]] constexpr size_t size() const noexcept { return size_; }
    [[nodiscard]] constexpr size_t size_bytes() const noexcept { return size_ * type_size; }
    [[nodiscard]] constexpr bool empty() const noexcept { return size_ == 0; }

    [[nodiscard]] constexpr const_reference operator[](const size_t i) const noexcept { if constexpr (i >= size_) { catastrophic_error("out of array size"); } return m_base[i]; }
    [[nodiscard]] constexpr reference operator[](const size_t i) noexcept { if constexpr (i >= size_) { catastrophic_error("out of array size"); } return m_base[i]; }

    [[nodiscard]] constexpr const_span view() const noexcept { return const_span(m_base, size_); }
    [[nodiscard]] constexpr span view() noexcept { return span(m_base, size_); }

    void fill(const uint8_t b) noexcept { _ministl_builtin_memset(m_base, b, size_); }
    void clear() noexcept { _ministl_builtin_memset(m_base, 0, size_); }

private:
    base_array<t_, size_>::type m_base;
};

}
