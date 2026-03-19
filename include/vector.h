#pragma once

#include <unique_ptr.h>

namespace framework {

template<typename t_>
class vector {
public:
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

    [[nodiscard]] const type& operator[](size_t index) const;
    [[nodiscard]] type& operator[](size_t index);

    void push_back(const_reference ref) requires(is_copy_constructible_v<t_>);
    void push_back(type&& ref) requires(is_move_constructible_v<t_>);

    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

private:
    static constexpr size_t default_capacity = 16;

    const_pointer data() const;
    pointer data();

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
    : m_data(new t_[capacity])
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
const vector<t_>::type& vector<t_>::operator[](const size_t index) const {
    if (index >= m_size) {
        catastrophic_error("index out of range");
    }

    return m_data[index];
}

template<typename t_>
vector<t_>::type& vector<t_>::operator[](const size_t index) {
    if (index >= m_size) {
        catastrophic_error("index out of range");
    }

    return m_data[index];
}

template<typename t_>
void vector<t_>::push_back(const_reference ref) requires(is_copy_constructible_v<t_>) {
    ensure_capacity(m_size + 1);
    new (&data()[m_size]) t_(ref);
    m_size++;
}

template<typename t_>
void vector<t_>::push_back(type&& ref) requires(is_move_constructible_v<t_>) {
    ensure_capacity(m_size + 1);
    new (&data()[m_size]) t_(framework::move(ref));
    m_size++;
}

template<typename t_>
vector<t_>::iterator vector<t_>::begin() {
    return iterator(&data()[0]);
}

template<typename t_>
vector<t_>::iterator vector<t_>::end() {
    return iterator(&data()[m_size]);
}

template<typename t_>
vector<t_>::const_iterator vector<t_>::cbegin() const {
    return iterator(&data()[0]);
}

template<typename t_>
vector<t_>::const_iterator vector<t_>::cend() const {
    return iterator(&data()[m_size]);
}

template<typename t_>
vector<t_>::const_pointer vector<t_>::data() const {
    return static_cast<const_pointer>(m_data);
}

template<typename t_>
vector<t_>::pointer vector<t_>::data() {
    return static_cast<pointer>(m_data);
}

template<typename t_>
void vector<t_>::ensure_capacity(const size_t capacity) {
    if (m_capacity >= capacity) {
        return;
    }

    if constexpr (is_move_constructible_v<t_>) {
        const auto* old_data = data();
        const auto* new_memory = new t_[capacity];

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
