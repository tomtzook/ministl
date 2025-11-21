#pragma once

#include "types.h"

namespace framework {

template<typename t_>
class span {
public:
    constexpr span() noexcept = default;
    constexpr span(t_* ptr, size_t size) noexcept;
    constexpr span(const span&) noexcept = default;
    constexpr span(span&&) noexcept = default;

    constexpr span& operator=(const span&) noexcept = default;
    constexpr span& operator=(span&&) noexcept = default;

    constexpr explicit operator bool() const noexcept;

    [[nodiscard]] constexpr const t_* data(size_t offset=0) const noexcept;
    [[nodiscard]] constexpr t_* data(size_t offset=0) noexcept;
    [[nodiscard]] constexpr size_t size_bytes() const noexcept;
    [[nodiscard]] constexpr size_t size() const noexcept;

private:
    t_* m_ptr = nullptr;
    size_t m_size = 0;
};

template<typename t_>
constexpr span<t_>::span(t_* ptr, const size_t size) noexcept
    : m_ptr(ptr), m_size(size)
{}

template<typename t_>
constexpr span<t_>::operator bool() const noexcept { return m_ptr != nullptr; }

template<typename t_>
constexpr const t_* span<t_>::data(size_t offset) const noexcept { return m_ptr + offset; }

template<typename t_>
constexpr t_* span<t_>::data(size_t offset) noexcept { return m_ptr + offset; }

template<typename t_>
constexpr size_t span<t_>::size_bytes() const noexcept { return m_size * sizeof(t_); }

template<typename t_>
constexpr size_t span<t_>::size() const noexcept { return m_size; }

}