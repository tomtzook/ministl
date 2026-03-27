#pragma once

namespace framework {

template<typename t_>
struct forward_iterator {
    using value_type = t_;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator = forward_iterator;

    explicit forward_iterator(pointer ptr);

    iterator& operator++();

    reference operator*();
    pointer operator->();

    bool operator==(const iterator& rhs) const;
    bool operator!=(const iterator& rhs) const;
    bool operator>=(const iterator& rhs) const;
    bool operator<=(const iterator& rhs) const;
    bool operator>(const iterator& rhs) const;
    bool operator<(const iterator& rhs) const;

private:
    pointer m_ptr;
};

template<typename t_>
using const_forward_iterator = forward_iterator<const t_>;

template<typename t_>
struct bidirectional_iterator {
    using value_type = t_;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator = bidirectional_iterator;

    explicit bidirectional_iterator(pointer ptr);

    iterator& operator++();
    iterator& operator--();

    reference operator*();
    pointer operator->();

    bool operator==(const iterator& rhs) const;
    bool operator!=(const iterator& rhs) const;
    bool operator>=(const iterator& rhs) const;
    bool operator<=(const iterator& rhs) const;
    bool operator>(const iterator& rhs) const;
    bool operator<(const iterator& rhs) const;

private:
    pointer m_ptr;
};

template<typename t_>
using const_bidirectional_iterator = bidirectional_iterator<const t_>;

template<typename t_>
struct random_access_iterator {
    using value_type = t_;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator = random_access_iterator;

    explicit random_access_iterator(pointer ptr);

    iterator& operator++();
    iterator& operator+=(size_t);
    iterator& operator--();
    iterator& operator-=(size_t);

    reference operator*();
    pointer operator->();

    bool operator==(const iterator& rhs) const;
    bool operator!=(const iterator& rhs) const;
    bool operator>=(const iterator& rhs) const;
    bool operator<=(const iterator& rhs) const;
    bool operator>(const iterator& rhs) const;
    bool operator<(const iterator& rhs) const;

private:
    pointer m_ptr;
};

template<typename t_>
using const_random_access_iterator = random_access_iterator<const t_>;

template<typename t_>
forward_iterator<t_>::forward_iterator(pointer ptr)
    : m_ptr(ptr)
{}

template<typename t_>
forward_iterator<t_>::iterator& forward_iterator<t_>::operator++() {
    ++m_ptr;
    return *this;
}

template<typename t_>
forward_iterator<t_>::reference forward_iterator<t_>::operator*() {
    return *m_ptr;
}

template<typename t_>
forward_iterator<t_>::pointer forward_iterator<t_>::operator->() {
    return m_ptr;
}

template<typename t_>
bool forward_iterator<t_>::operator==(const iterator& rhs) const {
    return m_ptr == rhs.m_ptr;
}

template<typename t_>
bool forward_iterator<t_>::operator!=(const iterator& rhs) const {
    return m_ptr != rhs.m_ptr;
}

template<typename t_>
bool forward_iterator<t_>::operator>=(const iterator& rhs) const {
    return m_ptr >= rhs.m_ptr;
}

template<typename t_>
bool forward_iterator<t_>::operator<=(const iterator& rhs) const {
    return m_ptr <= rhs.m_ptr;
}

template<typename t_>
bool forward_iterator<t_>::operator>(const iterator& rhs) const {
    return m_ptr > rhs.m_ptr;
}

template<typename t_>
bool forward_iterator<t_>::operator<(const iterator& rhs) const {
    return m_ptr < rhs.m_ptr;
}

template<typename t_>
bidirectional_iterator<t_>::bidirectional_iterator(pointer ptr)
    : m_ptr(ptr)
{}

template<typename t_>
bidirectional_iterator<t_>::iterator& bidirectional_iterator<t_>::operator++() {
    ++m_ptr;
    return *this;
}

template<typename t_>
bidirectional_iterator<t_>::iterator& bidirectional_iterator<t_>::operator--() {
    --m_ptr;
    return *this;
}

template<typename t_>
bidirectional_iterator<t_>::reference bidirectional_iterator<t_>::operator*() {
    return *m_ptr;
}

template<typename t_>
bidirectional_iterator<t_>::pointer bidirectional_iterator<t_>::operator->() {
    return m_ptr;
}

template<typename t_>
bool bidirectional_iterator<t_>::operator==(const iterator& rhs) const {
    return m_ptr == rhs.m_ptr;
}

template<typename t_>
bool bidirectional_iterator<t_>::operator!=(const iterator& rhs) const {
    return m_ptr != rhs.m_ptr;
}

template<typename t_>
bool bidirectional_iterator<t_>::operator>=(const iterator& rhs) const {
    return m_ptr >= rhs.m_ptr;
}

template<typename t_>
bool bidirectional_iterator<t_>::operator<=(const iterator& rhs) const {
    return m_ptr <= rhs.m_ptr;
}

template<typename t_>
bool bidirectional_iterator<t_>::operator>(const iterator& rhs) const {
    return m_ptr > rhs.m_ptr;
}

template<typename t_>
bool bidirectional_iterator<t_>::operator<(const iterator& rhs) const {
    return m_ptr < rhs.m_ptr;
}

template<typename t_>
random_access_iterator<t_>::random_access_iterator(pointer ptr)
    : m_ptr(ptr)
{}

template<typename t_>
random_access_iterator<t_>::iterator& random_access_iterator<t_>::operator++() {
    ++m_ptr;
    return *this;
}

template<typename t_>
random_access_iterator<t_>::iterator& random_access_iterator<t_>::operator+=(const size_t count) {
    m_ptr += count;
    return *this;
}

template<typename t_>
random_access_iterator<t_>::iterator& random_access_iterator<t_>::operator--() {
    --m_ptr;
    return *this;
}

template<typename t_>
random_access_iterator<t_>::iterator& random_access_iterator<t_>::operator-=(const size_t count) {
    m_ptr -= count;
    return *this;
}

template<typename t_>
random_access_iterator<t_>::reference random_access_iterator<t_>::operator*() {
    return *m_ptr;
}

template<typename t_>
random_access_iterator<t_>::pointer random_access_iterator<t_>::operator->() {
    return m_ptr;
}

template<typename t_>
bool random_access_iterator<t_>::operator==(const iterator& rhs) const {
    return m_ptr == rhs.m_ptr;
}

template<typename t_>
bool random_access_iterator<t_>::operator!=(const iterator& rhs) const {
    return m_ptr != rhs.m_ptr;
}

template<typename t_>
bool random_access_iterator<t_>::operator>=(const iterator& rhs) const {
    return m_ptr >= rhs.m_ptr;
}

template<typename t_>
bool random_access_iterator<t_>::operator<=(const iterator& rhs) const {
    return m_ptr <= rhs.m_ptr;
}

template<typename t_>
bool random_access_iterator<t_>::operator>(const iterator& rhs) const {
    return m_ptr > rhs.m_ptr;
}

template<typename t_>
bool random_access_iterator<t_>::operator<(const iterator& rhs) const {
    return m_ptr < rhs.m_ptr;
}

}
