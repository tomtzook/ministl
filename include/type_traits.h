#pragma once

namespace framework {

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

template<typename t_>
using remove_reference_t  = remove_reference<t_>::type;

template<typename t_>
struct remove_cv {
    using type = t_;
};

template<typename t_>
struct remove_cv<const t_> {
    using type = t_;
};

template<typename t_>
struct remove_cv<volatile t_> {
    using type = t_;
};

template<typename t_>
struct remove_cv<const volatile t_> {
    using type = t_;
};

template<typename t_>
using remove_cv_t  = remove_cv<t_>::type;

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

template<typename...>
using _void_t = void;

template<typename t_, typename = void>
struct _add_lvalue_reference_helper {
    using type = t_;
};

template<typename t_>
struct _add_lvalue_reference_helper<t_, _void_t<t_&>> {
    using type = t_&;
};

template<typename t_>
using _add_lval_ref_t = _add_lvalue_reference_helper<t_>::type;

template<typename t_, typename = void>
struct _add_rvalue_reference_helper {
    using type = t_;
};

template<typename t_>
struct _add_rvalue_reference_helper<t_, _void_t<t_&&>> {
    using type = t_&&;
};

template<typename t_>
using _add_rval_ref_t = _add_rvalue_reference_helper<t_>::type;

template<typename t_>
struct is_integral_helper : false_type {};

template<>
struct is_integral_helper<bool> : true_type {};

template<>
struct is_integral_helper<char> : true_type {};

template<>
struct is_integral_helper<unsigned char> : true_type {};

template<>
struct is_integral_helper<short> : true_type {};

template<>
struct is_integral_helper<unsigned short> : true_type {};

template<>
struct is_integral_helper<int> : true_type {};

template<>
struct is_integral_helper<unsigned int> : true_type {};

template<>
struct is_integral_helper<long> : true_type {};

template<>
struct is_integral_helper<unsigned long> : true_type {};

template<>
struct is_integral_helper<long long> : true_type {};

template<>
struct is_integral_helper<unsigned long long> : true_type {};

template<typename t_>
struct is_integral : is_integral_helper<remove_cv_t<t_>> {};

template<typename t_>
inline constexpr bool is_integral_v = is_integral<t_>::value;

template<typename t_>
struct is_floating_point_helper : false_type {};

template<>
struct is_floating_point_helper<float> : true_type {};

template<>
struct is_floating_point_helper<double> : true_type {};

template<typename t_>
struct is_floating_point : is_floating_point_helper<remove_cv_t<t_>> {};

template<typename t_>
inline constexpr bool is_floating_point_v = is_floating_point<t_>::value;

template<typename t_, class... _args>
inline constexpr bool is_constructible_v = __is_constructible(t_, _args...);
template<typename t_>
inline constexpr bool is_default_constructible_v = __is_constructible(t_);
template<typename t_>
inline constexpr bool is_copy_constructible_v = __is_constructible(t_, _add_lval_ref_t<const t_>);
template<typename t_>
inline constexpr bool is_move_constructible_v = __is_constructible(t_, _add_rval_ref_t<t_>);

template<typename t_, typename u_>
inline constexpr bool is_assignable_v = __is_assignable(t_, u_);
template<typename t_>
inline constexpr bool is_copy_assignable_v = __is_assignable(_add_lval_ref_t<t_>, _add_lval_ref_t<const t_>);
template<typename t_>
inline constexpr bool is_move_assignable_v = __is_assignable(_add_lval_ref_t<t_>, _add_rval_ref_t<t_>);

template<typename t_, class... _args>
inline constexpr bool is_trivially_constructible_v = __is_trivially_constructible(t_, _args...);
template<typename t_>
inline constexpr bool is_trivially_copyable_v = __is_trivially_copyable(t_);
template<typename t_>
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

template <typename t_, typename u_>
inline constexpr bool is_same_v = __is_same(t_, u_);

template <typename t_, typename u_>
inline constexpr bool is_convertible_v = __is_convertible(t_, u_);

template<typename t_>
struct _declval_protector {
    static const bool _stop = false;
};

template<typename t_, typename u_ = t_&&>
u_ _declval(int);

template<typename t_>
t_ _declval(long);

template<typename t_>
auto declval() noexcept -> decltype(_declval<t_>(0)) {
    static_assert(_declval_protector<t_>::_stop, "declval() must not be used!");
    return _declval<t_>(0);
}

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
