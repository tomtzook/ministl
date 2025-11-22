#pragma once

#include "types.h"

namespace framework {

template<typename t_>
class span {
public:
    using type = t_;
    using pointer = type*;
    using reference = type&;
    using const_pointer = const type*;
    using const_reference = const type&;

    static constexpr size_t type_size = sizeof(type);

    constexpr span() noexcept = default;
    // ReSharper disable once CppParameterMayBeConst
    constexpr span(pointer ptr, const size_t size) noexcept : m_ptr(ptr), m_size(size) {}
    constexpr span(const span&) noexcept = default;
    constexpr span(span&&) noexcept = default;

    constexpr span& operator=(const span&) noexcept = default;
    constexpr span& operator=(span&&) noexcept = default;

    constexpr explicit operator bool() const noexcept { return m_ptr != nullptr; }

    [[nodiscard]] constexpr const_pointer data(size_t offset=0) const noexcept { return m_ptr + offset; }
    [[nodiscard]] constexpr pointer data(size_t offset=0) noexcept { return m_ptr + offset; }
    [[nodiscard]] constexpr size_t size() const noexcept { return m_size; }
    [[nodiscard]] constexpr size_t size_bytes() const noexcept { return m_size * type_size; }
    [[nodiscard]] constexpr bool empty() const noexcept { return m_ptr == nullptr || m_size == 0; }

    [[nodiscard]] constexpr const_reference operator[](const size_t i) const noexcept { return m_ptr[i]; }
    [[nodiscard]] constexpr reference operator[](const size_t i) noexcept { return m_ptr[i]; }

private:
    pointer m_ptr = nullptr;
    size_t m_size = 0;
};

}