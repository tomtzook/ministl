#pragma once

#include "intrinsin.h"
#include "terminate.h"
#include "type_traits.h"
#include "result.h"
#include "verify.h"
#include "iterator.h"

namespace framework {

template<typename t_>
concept char_type = framework::is_same_v<t_, char> || framework::is_same_v<t_, wchar_t>;

template<char_type t_>
class string_base;

template<char_type t_>
class string_view_base {
public:
    using type = t_;
    using iterator = random_access_iterator<type>;
    using const_iterator = const_random_access_iterator<type>;

    string_view_base();
    // ReSharper disable once CppNonExplicitConvertingConstructor
    string_view_base(const type*); // NOLINT(*-explicit-constructor)
    // ReSharper disable once CppNonExplicitConvertingConstructor
    string_view_base(const string_base<type>&); // NOLINT(*-explicit-constructor)
    string_view_base(const string_view_base&);
    string_view_base(string_view_base&&) noexcept;

    string_view_base& operator=(const type*);
    string_view_base& operator=(const string_base<type>&);
    string_view_base& operator=(const string_view_base&);
    string_view_base& operator=(string_view_base&&) noexcept;

    const_iterator cbegin() const;
    const_iterator begin() const;
    iterator begin();
    const_iterator cend() const;
    const_iterator end() const;
    iterator end();

    [[nodiscard]] size_t length() const;
    [[nodiscard]] const type* data() const;
    [[nodiscard]] type* data();

private:
    type* m_data;
    size_t m_length;
};

template<char_type t_>
class string_base {
public:
    using type = t_;
    using iterator = random_access_iterator<type>;
    using const_iterator = const_random_access_iterator<type>;
    static constexpr size_t npos = static_cast<size_t>(-1);

    string_base();
    string_base(const string_base&) = delete;
    string_base(string_base&&) noexcept;
    ~string_base();

    string_base& operator=(const string_base&) = delete;
    string_base& operator=(string_base&&) noexcept;

    [[nodiscard]] const type& operator[](size_t) const;
    [[nodiscard]] type& operator[](size_t);

    [[nodiscard]] bool operator==(const string_base&) const;
    [[nodiscard]] bool operator!=(const string_base&) const;
    [[nodiscard]] bool operator>=(const string_base&) const;
    [[nodiscard]] bool operator<=(const string_base&) const;
    [[nodiscard]] bool operator>(const string_base&) const;
    [[nodiscard]] bool operator<(const string_base&) const;

    // ReSharper disable once CppNonExplicitConversionOperator
    [[nodiscard]] operator bool() const;

    const_iterator cbegin() const;
    const_iterator begin() const;
    iterator begin();
    const_iterator cend() const;
    const_iterator end() const;
    iterator end();

    size_t find(type ch) const;
    size_t find_last(type ch) const;

    [[nodiscard]] size_t length() const;
    [[nodiscard]] const type* c_str() const;
    [[nodiscard]] type* c_str();

    [[nodiscard]] result<> set(const type*);
    [[nodiscard]] result<> set(const type*, size_t, size_t);
    [[nodiscard]] result<> set(const string_view_base<type>&);
    [[nodiscard]] result<> set(const string_base&);
    [[nodiscard]] result<> reserve(size_t);
    void clear();

    [[nodiscard]] result<string_base> substr(size_t start, size_t end=-1) const;

    [[nodiscard]] static result<string_base> from(const type*);
    [[nodiscard]] static result<string_base> from(const type*, size_t, size_t);
    [[nodiscard]] static result<string_base> from(const string_view_base<type>&);
    [[nodiscard]] static result<string_base> from(const string_base&);

private:
    [[nodiscard]] result<> ensure_capacity(size_t);
    void destroy();

    int compare(const type* s1, const type* s2) const;

    type* m_data;
    size_t m_length;

    static constexpr auto inline_storage_size = 16 / sizeof(type);
    static constexpr auto inline_storage_mark = 0;
    union {
        type storage[inline_storage_size];
        size_t m_capacity;
    } m;
};

template<char_type t_>
string_view_base<t_>::string_view_base()
    : m_data(nullptr)
    , m_length(0)
{}

template<char_type t_>
string_view_base<t_>::string_view_base(const type* raw_str)
    : m_data(raw_str)
    , m_length(strlen(raw_str))
{}

template<char_type t_>
string_view_base<t_>::string_view_base(const string_base<type>& str)
    : m_data(str.c_str())
    , m_length(str.length())
{}

template<char_type t_>
string_view_base<t_>::string_view_base(const string_view_base& other)
    : m_data(other.m_data)
    , m_length(other.m_length)
{}

template<char_type t_>
string_view_base<t_>::string_view_base(string_view_base&& other) noexcept
    : m_data(other.m_data)
    , m_length(other.m_length)
{}

template<char_type t_>
string_view_base<t_>& string_view_base<t_>::operator=(const type* raw_str) {
    m_data = raw_str;
    m_length = strlen(raw_str);
    return *this;
}

template<char_type t_>
string_view_base<t_>& string_view_base<t_>::operator=(const string_base<type>& str) {
    m_data = str.c_str();
    m_length = str.length();
    return *this;
}

template<char_type t_>
string_view_base<t_>& string_view_base<t_>::operator=(const string_view_base& other) {
    m_data = other.m_data;
    m_length = other.m_length;
    return *this;
}

template<char_type t_>
string_view_base<t_>& string_view_base<t_>::operator=(string_view_base&& other) noexcept {
    m_data = other.m_data;
    m_length = other.m_length;
    return *this;
}

template<char_type t_>
string_view_base<t_>::const_iterator string_view_base<t_>::cbegin() const {
    return const_iterator(m_data);
}

template<char_type t_>
string_view_base<t_>::const_iterator string_view_base<t_>::begin() const {
    return const_iterator(m_data);
}

template<char_type t_>
string_view_base<t_>::iterator string_view_base<t_>::begin() {
    return iterator(m_data);
}

template<char_type t_>
string_view_base<t_>::const_iterator string_view_base<t_>::cend() const {
    return const_iterator(m_data != nullptr ? m_data + m_length : nullptr);
}

template<char_type t_>
string_view_base<t_>::const_iterator string_view_base<t_>::end() const {
    return const_iterator(m_data != nullptr ? m_data + m_length : nullptr);
}

template<char_type t_>
string_view_base<t_>::iterator string_view_base<t_>::end() {
    return iterator(m_data != nullptr ? m_data + m_length : nullptr);
}

template<char_type t_>
size_t string_view_base<t_>::length() const {
    return m_length;
}

template<char_type t_>
const string_view_base<t_>::type* string_view_base<t_>::data() const {
    return m_data;
}

template<char_type t_>
string_view_base<t_>::type* string_view_base<t_>::data() {
    return m_data;
}

template<char_type t_>
string_base<t_>::string_base()
    : m_data(nullptr)
    , m_length(0)
    , m{}
{}

template<char_type t_>
string_base<t_>::string_base(string_base&& other) noexcept
    : m_data()
    , m_length(other.m_length)
    , m(other.m) {
    if (other.m_data == other.m.storage) {
        m_data = m.storage;
    } else {
        m_data = other.m_data;
    }

    other.m_data = nullptr;
    other.m_length = 0;
    other.m = {};
}

template<char_type t_>
string_base<t_>::~string_base() {
    destroy();
}

template<char_type t_>
string_base<t_>& string_base<t_>::operator=(string_base&& other) noexcept {
    m_data = nullptr;
    m_length = other.m_length;
    m = other.m;

    if (other.m_data == other.m.storage) {
        m_data = m.storage;
    } else {
        m_data = other.m_data;
    }

    other.m_data = nullptr;
    other.m_length = 0;
    other.m = {};
    return *this;
}

template<char_type t_>
const string_base<t_>::type& string_base<t_>::operator[](const size_t index) const {
    if (index >= m_length)  catastrophic_error("out of string bound");
    return m_data[index];
}

template<char_type t_>
string_base<t_>::type& string_base<t_>::operator[](const size_t index) {
    if (index >= m_length)  catastrophic_error("out of string bound");
    return m_data[index];
}

template<char_type t_>
bool string_base<t_>::operator==(const string_base& other) const {
    return compare(m_data, other.m_data) == 0;
}

template<char_type t_>
bool string_base<t_>::operator!=(const string_base& other) const {
    return compare(m_data, other.m_data) != 0;
}

template<char_type t_>
bool string_base<t_>::operator>=(const string_base& other) const {
    return compare(m_data, other.m_data) >= 0;
}

template<char_type t_>
bool string_base<t_>::operator<=(const string_base& other) const {
    return compare(m_data, other.m_data) <= 0;
}

template<char_type t_>
bool string_base<t_>::operator>(const string_base& other) const {
    return compare(m_data, other.m_data) > 0;
}

template<char_type t_>
bool string_base<t_>::operator<(const string_base& other) const {
    return compare(m_data, other.m_data) < 0;
}

template<char_type t_>
string_base<t_>::operator bool() const {
    return m_length > 0;
}

template<char_type t_>
string_base<t_>::const_iterator string_base<t_>::cbegin() const {
    return const_iterator(m_data);
}

template<char_type t_>
string_base<t_>::const_iterator string_base<t_>::begin() const {
    return const_iterator(m_data);
}

template<char_type t_>
string_base<t_>::iterator string_base<t_>::begin() {
    return iterator(m_data);
}

template<char_type t_>
string_base<t_>::const_iterator string_base<t_>::cend() const {
    return const_iterator(m_data != nullptr ? m_data + m_length : nullptr);
}

template<char_type t_>
string_base<t_>::const_iterator string_base<t_>::end() const {
    return const_iterator(m_data != nullptr ? m_data + m_length : nullptr);
}

template<char_type t_>
string_base<t_>::iterator string_base<t_>::end() {
    return iterator(m_data != nullptr ? m_data + m_length : nullptr);
}

template<char_type t_>
size_t string_base<t_>::find(const type ch) const {
    for (int i = 0; i < m_length; i++) {
        if (m_data[i] == ch) {
            return i;
        }
    }

    return npos;
}

template<char_type t_>
size_t string_base<t_>::find_last(const type ch) const {
    for (int i = m_length - 1; i >= 0; i--) {
        if (m_data[i] == ch) {
            return i;
        }
    }

    return npos;
}

template<char_type t_>
size_t string_base<t_>::length() const {
    return m_length;
}

template<char_type t_>
const string_base<t_>::type* string_base<t_>::c_str() const {
    return m_data;
}

template<char_type t_>
string_base<t_>::type* string_base<t_>::c_str() {
    return m_data;
}

template<char_type t_>
result<> string_base<t_>::set(const type* raw_str) {
    const auto length = strlen(raw_str);
    return set(raw_str, 0, length);
}

template<char_type t_>
result<> string_base<t_>::set(const type* raw_str, const size_t start, const size_t length) {
    if (length < 1) {
        clear();
        return {};
    }

    verify(ensure_capacity(length + 1));
    memcpy(m_data, raw_str + start, length * sizeof(type));
    m_data[length] = '\0';

    m_length = length;

    return {};
}

template<char_type t_>
result<> string_base<t_>::set(const string_view_base<type>& view) {
    return set(view.data());
}

template<char_type t_>
result<> string_base<t_>::set(const string_base& other) {
    if (other.m_data) {
        return set(other.m_data);
    }

    clear();
    return {};
}

template<char_type t_>
result<> string_base<t_>::reserve(const size_t length) {
    return ensure_capacity(length);
}

template<char_type t_>
void string_base<t_>::clear() {
    if (m_data) {
        m_data[0] = '\0';
        m_length = 0;
    }
}

template<char_type t_>
result<string_base<t_>> string_base<t_>::substr(const size_t start, const size_t end) const {
    const auto actual_end = end == static_cast<size_t>(-1) ? m_length : end;

    if (start > actual_end) return err(status_bad_arg);
    if (actual_end > m_length) return err(status_bad_arg);

    const auto size = actual_end - start;

    string_base substr;
    verify(substr.reserve(size));
    memcpy(substr.m_data, m_data + start, size * sizeof(type));
    substr.m_length = size;

    return ok(move(substr));
}

template<char_type t_>
result<string_base<t_>> string_base<t_>::from(const type* raw_str) {
    string_base str;
    verify(str.set(raw_str));
    return ok(move(str));
}

template<char_type t_>
result<string_base<t_>> string_base<t_>::from(const type* raw_str, const size_t start, const size_t length) {
    string_base str;
    verify(str.set(raw_str, start, length));
    return ok(move(str));
}

template<char_type t_>
result<string_base<t_>> string_base<t_>::from(const string_view_base<type>& view) {
    string_base str;
    verify(str.set(view));
    return ok(move(str));
}

template<char_type t_>
result<string_base<t_>> string_base<t_>::from(const string_base& other) {
    string_base str;
    verify(str.set(other));
    return ok(move(str));
}

template<char_type t_>
result<> string_base<t_>::ensure_capacity(const size_t capacity) {
    const auto actual_capacity = capacity + 1;
    if (actual_capacity <= inline_storage_size && m_data != m.storage) {
        if (m_data) {
            const auto* last_data = m_data;
            m_data = m.storage;
            memcpy(m_data, last_data, (m_length + 1) * sizeof(type));

            delete m_data;
        } else {
            m_data = m.storage;
        }
    } else if (capacity > m.m_capacity) {
        auto* new_data = new type[actual_capacity];
        verify_alloc(new_data);

        memcpy(new_data, m_data, (m_length + 1) * sizeof(type));

        if (m_data && m_data != m.storage) {
            delete m_data;
        }

        m_data = new_data;
        m.m_capacity = capacity;
    }

    m_length = 0;
    m_data[0] = '\0';

    return {};
}

template<char_type t_>
void string_base<t_>::destroy() {
    if (m_data) {
        if (m_data != m.storage) {
            delete m_data;
        }

        m_data = nullptr;
        m_length = 0;
        m.m_capacity = 0;
    }
}

template<char_type t_>
int string_base<t_>::compare(const type* s1, const type* s2) const {
    if constexpr (is_same_v<type, char>) {
        return strcmp(s1, s2);
    } else if constexpr (is_same_v<type, wchar_t>) {
        return wcstrcmp(s1, s2);
    } else {
        static_assert(false, "unsupported type");
    }

    return 0;
}

using string_view = string_view_base<char>;
using wstring_view = string_view_base<wchar_t>;
using string = string_base<char>;
using wstring = string_base<wchar_t>;

}
