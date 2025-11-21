#pragma once

#include "memory.h"
#include "unique_ptr.h"
#include "verify.h"
#include "span.h"

namespace framework {

template<memory_type mem_t_>
class buffer_base {
public:
    constexpr buffer_base() = default;
    buffer_base(const buffer_base&) = delete;
    buffer_base(buffer_base&&) = default;
    explicit buffer_base(uint8_t* ptr, size_t size);

    buffer_base& operator=(const buffer_base&) = delete;
    buffer_base& operator=(buffer_base&&) = default;

    constexpr explicit operator bool() const;

    [[nodiscard]] constexpr size_t size() const;

    template<typename t_ = uint8_t>
    [[nodiscard]] const t_* data(size_t offset=0) const;
    template<typename t_ = uint8_t>
    [[nodiscard]] t_* data(size_t offset=0);

    template<typename t_ = uint8_t>
    [[nodiscard]] span<const t_> view() const noexcept;
    template<typename t_ = uint8_t>
    [[nodiscard]] span<t_> view() noexcept;

    static result<buffer_base> create(size_t size);
    static result<buffer_base> copy(const buffer_base&);

private:
    [[nodiscard]] const uint8_t* _get() const;
    [[nodiscard]] uint8_t* _get();

    struct deleter {
        void operator()(const uint8_t* t) const {
            delete[] t;
        }
    };

    using ptr_type = unique_ptr<uint8_t, deleter>;

    ptr_type m_ptr;
    size_t m_size = 0;
};

template<memory_type mem_t_>
buffer_base<mem_t_>::buffer_base(uint8_t* ptr, const size_t size)
    : m_ptr(ptr)
    , m_size(size)
{}

template<memory_type mem_t_>
constexpr buffer_base<mem_t_>::operator bool() const { return m_ptr; }

template<memory_type mem_t_>
constexpr size_t buffer_base<mem_t_>::size() const { return m_size; }

template<memory_type mem_t_>
template<typename t_>
const t_* buffer_base<mem_t_>::data(const size_t offset) const {
    if (offset * sizeof(t_) >= m_size) { abort("out of buffer range"); }
    return reinterpret_cast<const t_*>(_get()) + offset;
}

template<memory_type mem_t_>
template<typename t_>
t_* buffer_base<mem_t_>::data(const size_t offset) {
    if (offset * sizeof(t_) >= m_size) { abort("out of buffer range"); }
    return reinterpret_cast<t_*>(_get()) + offset;
}

template<memory_type mem_t_>
template<typename t_>
span<const t_> buffer_base<mem_t_>::view() const noexcept { return {reinterpret_cast<const t_*>(m_ptr.get()), m_size / sizeof(t_)}; }

template<memory_type mem_t_>
template<typename t_>
span<t_> buffer_base<mem_t_>::view() noexcept { return {reinterpret_cast<t_*>(m_ptr.get()), m_size / sizeof(t_)}; }

template<memory_type mem_t_>
result<buffer_base<mem_t_>> buffer_base<mem_t_>::create(const size_t size) {
    // ReSharper disable once CppDFAMemoryLeak
    auto ptr = new uint8_t[size];
    verify_alloc(ptr);

    // ReSharper disable once CppDFAMemoryLeak
    return buffer_base(ptr, size);
}

template<memory_type mem_t_>
result<buffer_base<mem_t_>> buffer_base<mem_t_>::copy(const buffer_base& other) {
    auto new_buff = verify(create(other.size()));
    memcpy(new_buff.data(), other.data(), other.size());

    return framework::move(new_buff);
}

template<memory_type mem_t_>
const uint8_t* buffer_base<mem_t_>::_get() const { return m_ptr.get(); }

template<memory_type mem_t_>
uint8_t* buffer_base<mem_t_>::_get() { return m_ptr.get(); }

using data_buffer = buffer_base<memory_type::data>;
using code_buffer = buffer_base<memory_type::code>;
using buffer = data_buffer;

}
