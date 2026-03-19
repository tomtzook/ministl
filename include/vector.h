#pragma once

#include "types.h"
#include "type_traits.h"
#include "result.h"

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
    explicit vector(size_t capacity);
    ~vector();

    vector(const vector&) = delete;
    vector(vector&&) = delete;

    vector& operator=(const vector&) = delete;
    vector& operator=(vector&&) = delete;

    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t capacity() const;

    [[nodiscard]] const_reference front() const;
    [[nodiscard]] reference front();
    [[nodiscard]] const_reference back() const;
    [[nodiscard]] reference back();

    [[nodiscard]] const_reference operator[](size_t index) const;
    [[nodiscard]] reference operator[](size_t index);

    void push_front(const_reference ref) requires(is_copy_constructible_v<t_>);
    void push_front(type&& ref) requires(is_move_constructible_v<t_>);
    void pop_front();
    void push_back(const_reference ref) requires(is_copy_constructible_v<t_>);
    void push_back(type&& ref) requires(is_move_constructible_v<t_>);
    void pop_back();

    void insert(const iterator& it, const_reference ref) requires(is_copy_constructible_v<t_>);
    void insert(const iterator& it, type&& ref) requires(is_move_constructible_v<t_>);
    iterator erase(const iterator& it);

    void reserve(size_t capacity);

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    const_pointer data(size_t offset=0) const;
    pointer data(size_t offset=0);
    size_t find_iterator_index(iterator it) const;

    void shift_elements_right(size_t start_index);
    void shift_elements_left(size_t start_index);
    void ensure_capacity(size_t capacity);
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
    : vector(default_capacity)
{}

template<typename t_>
vector<t_>::vector(const size_t capacity)
    : m_data(new uint8_t[capacity * sizeof(type)])
    , m_size(0)
    , m_capacity(capacity)
{}

template<typename t_>
vector<t_>::~vector() {
    destruct_elements();
    delete[] m_data;
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
void vector<t_>::push_front(const_reference ref) requires(is_copy_constructible_v<t_>) {
    insert(begin(), ref);
}

template<typename t_>
void vector<t_>::push_front(type&& ref) requires(is_move_constructible_v<t_>) {
    insert(begin(), framework::move(ref));
}

template<typename t_>
void vector<t_>::pop_front() {
    if (m_size < 1) {
        return;
    }

    erase(begin());
}

template<typename t_>
void vector<t_>::push_back(const_reference ref) requires(is_copy_constructible_v<t_>) {
    insert(end(), ref);
}

template<typename t_>
void vector<t_>::push_back(type&& ref) requires(is_move_constructible_v<t_>) {
    insert(end(), framework::move(ref));
}

template<typename t_>
void vector<t_>::pop_back() {
    if (m_size < 1) {
        return;
    }

    auto it = --end();
    erase(it);
}

template<typename t_>
void vector<t_>::insert(const iterator& it, const_reference ref) requires(is_copy_constructible_v<t_>) {
    const auto index = find_iterator_index(it);
    ensure_capacity(m_size + 1);
    shift_elements_right(index);

    new (data(index)) t_(ref);
    m_size++;
}

template<typename t_>
void vector<t_>::insert(const iterator& it, type&& ref) requires(is_move_constructible_v<t_>) {
    const auto index = find_iterator_index(it);
    ensure_capacity(m_size + 1);
    shift_elements_right(index);

    new (data(index)) t_(framework::move(ref));
    m_size++;
}

template<typename t_>
vector<t_>::iterator vector<t_>::erase(const iterator& it) {
    if (m_size < 1) {
        return end();
    }

    const auto index = find_iterator_index(it);

    data()[index].~t_();
    shift_elements_left(index);
    m_size--;

    return iterator(data(index));
}

template<typename t_>
void vector<t_>::reserve(const size_t capacity) {
    ensure_capacity(capacity);
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
vector<t_>::const_iterator vector<t_>::cbegin() const {
    return iterator(data());
}

template<typename t_>
vector<t_>::const_iterator vector<t_>::cend() const {
    return iterator(data(m_size));
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
    for (int i = m_size - 1; i >= static_cast<int>(start_index); i--) {
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
void vector<t_>::ensure_capacity(const size_t capacity) {
    if (m_capacity >= capacity) {
        return;
    }

    if constexpr (is_move_constructible_v<t_>) {
        auto* old_data = data();
        auto* new_memory = new uint8_t[capacity * sizeof(type)];

        for (size_t i = 0; i < m_size; i++) {
            new (&new_memory[i]) t_(framework::move(old_data[i]));
        }

        replace_data(new_memory);
        m_capacity = capacity;
    } else {
        static_assert(false, "vector element type must be move constructible");
    }

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
}

}
