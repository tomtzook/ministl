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

    [[nodiscard]] constexpr const_pointer data(const size_t offset=0) const noexcept { if (offset >= m_size) { catastrophic_error("out of span size"); } return m_ptr + offset; }
    [[nodiscard]] constexpr pointer data(const size_t offset=0) noexcept { if (offset >= m_size) { catastrophic_error("out of span size"); } return m_ptr + offset; }
    [[nodiscard]] constexpr size_t size() const noexcept { return m_size; }
    [[nodiscard]] constexpr size_t size_bytes() const noexcept { return m_size * type_size; }
    [[nodiscard]] constexpr bool empty() const noexcept { return m_ptr == nullptr || m_size == 0; }

    [[nodiscard]] constexpr const_pointer front() const noexcept { return m_ptr; }
    [[nodiscard]] constexpr pointer front() noexcept { return m_ptr; }
    [[nodiscard]] constexpr const_pointer last() const noexcept { return m_ptr + m_size - 1; }
    [[nodiscard]] constexpr pointer last() noexcept { return m_ptr + m_size - 1; }

    [[nodiscard]] constexpr const_reference operator[](const size_t i) const noexcept { if (i >= m_size) { catastrophic_error("out of span size"); } return m_ptr[i]; }
    [[nodiscard]] constexpr reference operator[](const size_t i) noexcept { if (i >= m_size) { catastrophic_error("out of span size"); } return m_ptr[i]; }

    [[nodiscard]] constexpr span<const remove_cv_t<t_>> sub(const size_t offset, const size_t size) const noexcept {
        if (offset >= m_size) { catastrophic_error("out of span size") }
        if (size >= m_size - offset) { catastrophic_error("out of span size") }
        return span<const remove_cv_t<t_>>(m_ptr + offset, size);
    }

    [[nodiscard]] constexpr span sub(const size_t offset, const size_t size) noexcept {
        if (offset >= m_size) { catastrophic_error("out of span size") }
        if (size >= m_size - offset) { catastrophic_error("out of span size") }
        return span(m_ptr + offset, size);
    }

private:
    pointer m_ptr = nullptr;
    size_t m_size = 0;
};

}