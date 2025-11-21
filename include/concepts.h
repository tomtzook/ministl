#pragma once

#include "type_traits.h"

namespace framework {

template<typename t_>
concept trivially_constructible = is_trivially_constructible_v<t_>;

template<typename t_>
concept trivially_copyable = is_trivially_copyable_v<t_>;

template<typename t_>
concept trivially_destructible = is_trivially_destructible_v<t_>;

template<typename t_, typename u_>
concept same_as = is_same_v<t_, u_>;

template<typename t_>
concept integral = is_integral_v<t_>;

}
