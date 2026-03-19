#pragma once

#include <concepts.h>

namespace framework {

template<integral t_>
constexpr t_ max(const t_ value1, const t_ value2) noexcept {
    return value1 > value2 ? value1 : value2;
}

template<integral t_>
constexpr t_ min(const t_ value1, const t_ value2) noexcept {
    return value1 > value2 ? value2 : value1;
}

template<integral t_>
constexpr t_ round_up(const t_ value, const t_ step) noexcept {
    return (value + step - 1) / step * step;
}

template<integral t_>
constexpr t_ round_down(const t_ value, const t_ step) noexcept {
    return (value / step) * step;
}

}
