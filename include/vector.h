#pragma once

#include "type_traits.h"
#include "result.h"
#include "math.h"

namespace framework {

template<typename t_>
class vector {
public:
    static constexpr size_t default_capacity = 16;

    using type = t_;
    using pointer = type*;
    using reference = type&;
    using const_pointer = const type*;
    using const_reference = const type&;

    class iterator {
    public:
        explicit iterator(pointer ptr);

        iterator& operator++();
        iterator& operator--();

        reference operator*();
        pointer operator->();

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

    private:
        pointer m_ptr;
    };

    class const_iterator {
    public:
        explicit const_iterator(const_pointer ptr);

        const_iterator& operator++();
        const_iterator& operator--();

        const_reference operator*();
        const_pointer operator->();

        bool operator==(const const_iterator& rhs) const;
        bool operator!=(const const_iterator& rhs) const;

    private:
        const_pointer m_ptr;
    };

    vector();
    vector(const vector&) = delete;
    vector(vector&&) noexcept;
    ~vector();

    vector& operator=(const vector&) = delete;
    vector& operator=(vector&&) noexcept;

    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t capacity() const;

    [[nodiscard]] const_reference front() const;
    [[nodiscard]] reference front();
    [[nodiscard]] const_reference back() const;
    [[nodiscard]] reference back();

    [[nodiscard]] const_reference operator[](size_t index) const;
    [[nodiscard]] reference operator[](size_t index);

    result<> push_front(const_reference ref) requires(is_copy_constructible_v<t_>);
    result<> push_front(type&& ref) requires(is_move_constructible_v<t_>);
    result<> pop_front();
    result<> push_back(const_reference ref) requires(is_copy_constructible_v<t_>);
    result<> push_back(type&& ref) requires(is_move_constructible_v<t_>);
    result<> pop_back();

    result<iterator> insert(const iterator& it, const_reference ref) requires(is_copy_constructible_v<t_>);
    result<iterator> insert(const iterator& it, type&& ref) requires(is_move_constructible_v<t_>);
    result<iterator> erase(const iterator& it);

    result<> reserve(size_t capacity);
    result<> copy(const vector& other);
    result<> clear();

    const_iterator cbegin() const;
    const_iterator cend() const;
    const_iterator begin() const;
    const_iterator end() const;
    iterator begin();
    iterator end();

    static result<vector> create(size_t capacity=default_capacity);
    static result<vector> copy(const vector&) requires(is_copy_constructible_v<t_>);

private:
    const_pointer data(size_t offset=0) const;
    pointer data(size_t offset=0);
    size_t find_iterator_index(iterator it) const;

    void shift_elements_right(size_t start_index);
    void shift_elements_left(size_t start_index);

    result<> create_elements(size_t capacity);
    result<> copy_elements(const uint8_t* other_data, size_t size);

    result<> ensure_allocated();
    result<> ensure_capacity(size_t capacity, bool expand_exact=true);
    void replace_data(uint8_t* new_data);
    void destruct_elements();

    uint8_t* m_data;
    size_t m_size;
    size_t m_capacity;
};

template<typename t_>
vector<t_>::iterator::iterator(pointer ptr)
    : m_ptr(ptr)
{}

template<typename t_>
vector<t_>::iterator& vector<t_>::iterator::operator++() {
    ++m_ptr;
    return *this;
}

template<typename t_>
vector<t_>::iterator& vector<t_>::iterator::operator--() {
    --m_ptr;
    return *this;
}

template<typename t_>
vector<t_>::reference vector<t_>::iterator::operator*() {
    return *m_ptr;
}

template<typename t_>
vector<t_>::pointer vector<t_>::iterator::operator->() {
    return m_ptr;
}

template<typename t_>
bool vector<t_>::iterator::operator==(const iterator& rhs) const {
    return m_ptr == rhs.m_ptr;
}

template<typename t_>
bool vector<t_>::iterator::operator!=(const iterator& rhs) const {
    return m_ptr != rhs.m_ptr;
}

template<typename t_>
vector<t_>::const_iterator::const_iterator(const_pointer ptr)
    : m_ptr(ptr)
{}

template<typename t_>
vector<t_>::const_iterator& vector<t_>::const_iterator::operator++() {
    ++m_ptr;
    return *this;
}

template<typename t_>
vector<t_>::const_iterator& vector<t_>::const_iterator::operator--() {
    --m_ptr;
    return *this;
}

template<typename t_>
vector<t_>::const_reference vector<t_>::const_iterator::operator*() {
    return *m_ptr;
}

template<typename t_>
vector<t_>::const_pointer vector<t_>::const_iterator::operator->() {
    return m_ptr;
}

template<typename t_>
bool vector<t_>::const_iterator::operator==(const const_iterator& rhs) const {
    return m_ptr == rhs.m_ptr;
}

template<typename t_>
bool vector<t_>::const_iterator::operator!=(const const_iterator& rhs) const {
    return m_ptr != rhs.m_ptr;
}

template<typename t_>
vector<t_>::vector()
    : m_data(nullptr)
    , m_size(0)
    , m_capacity(0)
{}

template<typename t_>
vector<t_>::vector(vector&& other) noexcept
    : m_data(other.m_data)
    , m_size(other.m_size)
    , m_capacity(other.m_capacity) {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template<typename t_>
vector<t_>::~vector() {
    if (m_data != nullptr) {
        destruct_elements();
        delete[] m_data;
    }
}

template<typename t_>
vector<t_>& vector<t_>::operator=(vector&& other) noexcept {
    m_data = other.m_data;
    m_size = other.m_size;
    m_capacity = other.m_capacity;

    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;

    return *this;
}

template<typename t_>
size_t vector<t_>::size() const {
    return m_size;
}

template<typename t_>
size_t vector<t_>::capacity() const {
    return m_capacity;
}

template<typename t_>
vector<t_>::const_reference vector<t_>::front() const {
    return this->operator[](0);
}

template<typename t_>
vector<t_>::reference vector<t_>::front() {
    return this->operator[](0);
}

template<typename t_>
vector<t_>::const_reference vector<t_>::back() const {
    return this->operator[](m_size > 0 ? m_size - 1 : 0);
}

template<typename t_>
vector<t_>::reference vector<t_>::back() {
    return this->operator[](m_size > 0 ? m_size - 1 : 0);
}

template<typename t_>
vector<t_>::const_reference vector<t_>::operator[](const size_t index) const {
    if (index >= m_size) {
        catastrophic_error("index out of range");
    }

    return *data(index);
}

template<typename t_>
vector<t_>::reference vector<t_>::operator[](const size_t index) {
    if (index >= m_size) {
        catastrophic_error("index out of range");
    }

    return *data(index);
}

template<typename t_>
result<> vector<t_>::push_front(const_reference ref) requires(is_copy_constructible_v<t_>) {
    verify(ensure_allocated());
    verify(insert(begin(), ref));
    return {};
}

template<typename t_>
result<> vector<t_>::push_front(type&& ref) requires(is_move_constructible_v<t_>) {
    verify(ensure_allocated());
    verify(insert(begin(), framework::move(ref)));
    return {};
}

template<typename t_>
result<> vector<t_>::pop_front() {
    if (m_size < 1) {
        return {};
    }

    verify(ensure_allocated());
    return erase(begin());
}

template<typename t_>
result<> vector<t_>::push_back(const_reference ref) requires(is_copy_constructible_v<t_>) {
    verify(ensure_allocated());
    verify(insert(end(), ref));
    return {};
}

template<typename t_>
result<> vector<t_>::push_back(type&& ref) requires(is_move_constructible_v<t_>) {
    verify(ensure_allocated());
    verify(insert(end(), framework::move(ref)));
    return {};
}

template<typename t_>
result<> vector<t_>::pop_back() {
    if (m_size < 1) {
        return {};
    }

    verify(ensure_allocated());
    auto it = --end();
    return erase(it);
}

template<typename t_>
result<typename vector<t_>::iterator> vector<t_>::insert(const iterator& it, const_reference ref) requires(is_copy_constructible_v<t_>) {
    verify(ensure_allocated());

    const auto index = find_iterator_index(it);
    verify(ensure_capacity(m_size + 1));
    shift_elements_right(index);

    new (data(index)) t_(ref);
    m_size++;

    return ok(iterator(data(index)));
}

template<typename t_>
result<typename vector<t_>::iterator> vector<t_>::insert(const iterator& it, type&& ref) requires(is_move_constructible_v<t_>) {
    verify(ensure_allocated());

    const auto index = find_iterator_index(it);
    verify(ensure_capacity(m_size + 1));
    shift_elements_right(index);

    new (data(index)) t_(framework::move(ref));
    m_size++;

    return ok(iterator(data(index)));
}

template<typename t_>
result<typename vector<t_>::iterator> vector<t_>::erase(const iterator& it) {
    verify(ensure_allocated());

    if (m_size < 1) {
        return ok(end());
    }

    const auto index = find_iterator_index(it);

    data()[index].~t_();
    shift_elements_left(index);
    m_size--;

    return ok(iterator(data(index)));
}

template<typename t_>
result<> vector<t_>::reserve(const size_t capacity) {
    return ensure_capacity(capacity, true);
}

template<typename t_>
result<> vector<t_>::copy(const vector& other) {
    return copy_elements(other.m_data, other.m_size);
}

template<typename t_>
result<> vector<t_>::clear() {
    destruct_elements();
    return {};
}

template<typename t_>
vector<t_>::const_iterator vector<t_>::cbegin() const {
    return iterator(data());
}

template<typename t_>
vector<t_>::const_iterator vector<t_>::cend() const {
    return iterator(data(m_size));
}

template<typename t_>
vector<t_>::const_iterator vector<t_>::begin() const {
    return const_iterator(data());
}

template<typename t_>
vector<t_>::const_iterator vector<t_>::end() const {
    return const_iterator(data(m_size));
}

template<typename t_>
vector<t_>::iterator vector<t_>::begin() {
    return iterator(data());
}

template<typename t_>
vector<t_>::iterator vector<t_>::end() {
    return iterator(data(m_size));
}

template<typename t_>
result<vector<t_>> vector<t_>::create(const size_t capacity) {
    vector vec;
    verify(vec.create_elements(capacity));
    return ok(move(vec));
}

template<typename t_>
result<vector<t_>> vector<t_>::copy(const vector& other) requires(is_copy_constructible_v<t_>) {
    auto vec = verify(create(other.m_capacity));
    verify(vec.copy_elements(other.m_data, other.m_size));
    return ok(move(vec));
}

template<typename t_>
vector<t_>::const_pointer vector<t_>::data(const size_t offset) const {
    return reinterpret_cast<const_pointer>(m_data) + offset;
}

template<typename t_>
vector<t_>::pointer vector<t_>::data(const size_t offset) {
    return reinterpret_cast<pointer>(m_data) + offset;
}

template<typename t_>
size_t vector<t_>::find_iterator_index(iterator it) const {
    const auto* ptr = it.operator->();

    const auto* data_start = data();
    const auto* data_end = data(m_size);
    if (ptr < data_start || ptr > data_end) {
        catastrophic_error("bad iterator");
    }

    return ptr - data_start;
}

template<typename t_>
void vector<t_>::shift_elements_right(const size_t start_index) {
    auto* data_ptr = data();
    for (int i = m_size; i >= static_cast<int>(start_index); i--) {
        new (&data_ptr[i]) t_(framework::move(data_ptr[i-1]));
        data_ptr[i-1].~t_();
    }
}

template<typename t_>
void vector<t_>::shift_elements_left(const size_t start_index) {
    auto* data_ptr = data();
    for (auto i = start_index; i < m_size - 1; i++) {
        new (&data_ptr[i]) t_(framework::move(data_ptr[i+1]));
        data_ptr[i+1].~t_();
    }
}

template<typename t_>
result<> vector<t_>::create_elements(const size_t capacity) {
    m_data = new uint8_t[capacity * sizeof(type)];
    verify_alloc(m_data);

    m_size = 0;
    m_capacity = capacity;

    return {};
}

template<typename t_>
result<> vector<t_>::copy_elements(const uint8_t* other_data, const size_t size) {
    destruct_elements();

    verify(ensure_capacity(size));
    m_size = 0;

    const auto* this_ptr = data();
    const auto* other_ptr = reinterpret_cast<const t_*>(other_data);
    for (size_t i = 0; i < size; i++) {
        new (&this_ptr[i]) t_(other_ptr[i+1]);
        m_size++;
    }

    return {};
}

template<typename t_>
result<> vector<t_>::ensure_allocated() {
    if (m_data) {
        return {};
    }

    return create_elements(default_capacity);
}

template<typename t_>
result<> vector<t_>::ensure_capacity(const size_t capacity, const bool expand_exact) {
    if (m_capacity >= capacity) {
        return {};
    }

    const auto new_capacity = expand_exact ? capacity : max(capacity, m_capacity * 2);

    if constexpr (is_move_constructible_v<t_>) {
        auto* old_data = data();
        auto* new_memory = new uint8_t[new_capacity * sizeof(type)];
        verify_alloc(new_memory);

        for (size_t i = 0; i < m_size; i++) {
            new (&new_memory[i]) t_(framework::move(old_data[i]));
        }

        replace_data(new_memory);
        m_capacity = new_capacity;
    } else {
        static_assert(false, "vector element type must be move constructible");
    }

    return {};
}

template<typename t_>
void vector<t_>::replace_data(uint8_t* new_data) {
    destruct_elements();

    const auto* old_data = data();
    m_data = new_data;

    delete[] old_data;
}

template<typename t_>
void vector<t_>::destruct_elements() {
    const auto* data_ptr = data();
    for (size_t i = 0; i < m_size; i++) {
        data_ptr[i].~t_();
    }
    m_size = 0;
}

}
