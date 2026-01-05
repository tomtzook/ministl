#pragma once

#include <concepts.h>

namespace framework {

template<integral t_>
constexpr t_ round_up(const t_ value, const t_ step) noexcept {
    return (value + step - 1) / step * step;
}

template<integral t_>
constexpr t_ round_down(const t_ value, const t_ step) noexcept {
    return (value / step) * step;
}

}
