#pragma once

#include "result.h"

namespace framework {

template<typename t_>
struct default_deleter {
    void operator()(const t_* t) const {
        delete t;
    }
};

template<typename t_>
struct array_deleter {
    void operator()(const t_* t) const {
        delete[] t;
    }
};

template<typename t_>
struct mem_free_deleter {
    void operator()(const t_* t) const {
        framework::free(t);
    }
};

template<typename t_, typename = default_deleter<t_>>
class unique_ptr {
public:
    using type = t_;
    using pointer = type*;
    using reference = type&;
    using move_ref = type&&;
    using const_pointer = const type*;
    using const_reference = const type&;
    using const_move_ref = const type&&;

    constexpr unique_ptr() noexcept = default;
    unique_ptr(const unique_ptr&) = delete;
    // ReSharper disable once CppParameterMayBeConst
    constexpr explicit unique_ptr(pointer ptr) noexcept : m_ptr(ptr) {}
    constexpr unique_ptr(unique_ptr&& other) noexcept : m_ptr(other.m_ptr) { other.m_ptr = nullptr; }
    ~unique_ptr() noexcept { reset(); }

    unique_ptr& operator=(const unique_ptr&) = delete;
    unique_ptr& operator=(unique_ptr&&) noexcept;

    constexpr explicit operator bool() const { return m_ptr != nullptr; }

    unique_ptr& operator=(const_pointer new_ptr) noexcept { reset(new_ptr); return *this; }

    constexpr reference operator*() & { return *_get(); }
    constexpr const_reference operator*() const & { return *_get(); }
    constexpr move_ref operator*() && { return framework::move(_get()); }
    constexpr const_move_ref operator*() const && { return framework::move(_get()); }
    constexpr pointer operator->() { return _get(); }
    constexpr const_pointer operator->() const { return _get(); }

    [[nodiscard]] constexpr bool has_value() const noexcept { return m_ptr != nullptr; }
    [[nodiscard]] const_pointer get() const noexcept { return _get(); }
    [[nodiscard]] pointer get() noexcept { return _get(); }

    void reset(pointer new_ptr = nullptr);
    [[nodiscard]] pointer release();

    template<typename... args_>
    static result<unique_ptr> create(args_... args);
    template<size_t align_, typename... args_>
    static result<unique_ptr> create(args_... args);

private:
    [[nodiscard]] const_pointer _get() const { if (m_ptr == nullptr) { abort("empty unique_ptr"); } return m_ptr; }
    [[nodiscard]] pointer _get() { if (m_ptr == nullptr) { abort("empty unique_ptr"); } return m_ptr; }

    pointer m_ptr = nullptr;
};

template<typename t_, typename deleter_>
unique_ptr<t_, deleter_>& unique_ptr<t_, deleter_>::operator=(unique_ptr&& other) noexcept {
    m_ptr = other.m_ptr;
    other.m_ptr = nullptr;
    return *this;
}

template<typename t_, typename deleter_>
void unique_ptr<t_, deleter_>::reset(pointer new_ptr) {
    if (m_ptr != nullptr) {
        deleter_()(m_ptr);
        m_ptr = nullptr;
    }

    m_ptr = new_ptr;
}

template<typename t_, typename deleter_>
unique_ptr<t_, deleter_>::pointer unique_ptr<t_, deleter_>::release() {
    auto ptr = _get();
    m_ptr = nullptr;
    return ptr;
}

template<typename t_, typename deleter_>
template<typename... args_>
result<unique_ptr<t_, deleter_>> unique_ptr<t_, deleter_>::create(args_... args) {
    auto ptr = new t_(args...);
    return framework::ok(unique_ptr(ptr));
}

template<typename t_, typename deleter_>
template<size_t align_, typename... args_>
result<unique_ptr<t_, deleter_>> unique_ptr<t_, deleter_>::create(args_... args) {
    auto ptr = new (align_) t_(args...);
    return framework::ok(unique_ptr(ptr));
}

}
