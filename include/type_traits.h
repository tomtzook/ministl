#pragma once

namespace framework {

template<class t_, t_ _v>
struct integral_constant {
    using value_type = t_;
    using type = integral_constant;
    static constexpr t_ value = _v;

    // ReSharper disable once CppNonExplicitConversionOperator
    constexpr operator value_type() const { return value; } // NOLINT(*-explicit-constructor)
    constexpr value_type operator()() const { return value; }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template<class t_, class... _args>
struct is_trivially_constructible
    : integral_constant<bool, __is_trivially_constructible(t_, _args...)> {};

template<class t_, class... _args>
inline constexpr bool is_trivially_constructible_v = __is_trivially_constructible(t_, _args...);

template<class t_>
struct is_trivially_copyable : integral_constant<bool, __is_trivially_copyable(t_)> {};

template<class t_>
inline constexpr bool is_trivially_copyable_v = __is_trivially_copyable(t_);

template<class t_>
struct is_trivially_destructible : integral_constant<bool, __is_trivially_destructible(t_)> {};

template<class t_>
inline constexpr bool is_trivially_destructible_v = __is_trivially_destructible(t_);

template<bool, typename = void>
struct enable_if {
};

template<typename t_>
struct enable_if<true, t_> {
    using type = t_;
};

template<bool _cond, typename t_ = void>
using enable_if_t = enable_if<_cond, t_>::type;

template<class t_>
struct remove_reference {
    using type = t_;
};

template<class t_>
struct remove_reference<t_&> {
    using type = t_;
};

template<class t_>
struct remove_reference<t_&&> {
    using type = t_;
};

template <typename t_, typename u_>
inline constexpr bool is_same_v = __is_same(t_, u_);

template<typename t_, typename t2_>
constexpr bool is_any_of(t_ first, t2_ second) {
    return first == second;
}

template<typename t_, typename t2_, typename... _args>
constexpr bool is_any_of(t_ first, t2_ second, _args... args) {
    return first == second || is_any_of(first, args...);
}

template<typename t_>
constexpr remove_reference<t_>::type&& move(t_&& arg) {
    return static_cast<remove_reference<t_>::type&&>(arg);
}

}
