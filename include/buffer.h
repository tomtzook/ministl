#pragma once

#include "intrinsin.h"
#include "memory.h"
#include "unique_ptr.h"
#include "verify.h"
#include "span.h"

namespace framework {

template<memory_type mem_t_>
class buffer_base {
public:
    using type = uint8_t;
    using const_pointer = const type*;
    using const_reference = const type&;
    using pointer = type*;
    using reference = type&;

    static constexpr auto mem_type = mem_t_;

    constexpr buffer_base() = default;
    buffer_base(const buffer_base&) = delete;
    buffer_base(buffer_base&&) = default;
    explicit buffer_base(void* ptr, size_t size);

    buffer_base& operator=(const buffer_base&) = delete;
    buffer_base& operator=(buffer_base&&) = default;

    constexpr explicit operator bool() const;

    [[nodiscard]] constexpr size_t size() const;

    template<typename t_ = uint8_t>
    [[nodiscard]] const t_* data(size_t offset=0) const;
    template<typename t_ = uint8_t>
    [[nodiscard]] t_* data(size_t offset=0);

    [[nodiscard]] const_reference operator[](const size_t i) const noexcept { if (i >= m_size) { catastrophic_error("out of buffer size"); } return _get()[i]; }
    [[nodiscard]] reference operator[](const size_t i) noexcept { if (i >= m_size) { catastrophic_error("out of buffer size"); } return _get()[i]; }

    template<typename t_ = uint8_t>
    [[nodiscard]] span<const t_> view() const noexcept;
    template<typename t_ = uint8_t>
    [[nodiscard]] span<t_> view() noexcept;

    template<typename t_ = uint8_t>
    [[nodiscard]] span<const t_> sub_view(size_t offset, size_t size) const noexcept;
    template<typename t_ = uint8_t>
    [[nodiscard]] span<t_> sub_view(size_t offset, size_t size) noexcept;

    void fill(uint8_t b);
    void clear();

    template<size_t align_ = default_alignment>
    result<> resize(size_t new_size);

    template<size_t align_ = default_alignment>
    result<buffer_base> copy() const;
    template<size_t align_ = default_alignment>
    result<buffer_base> sub(size_t offset, size_t size) const;

    template<size_t align_ = default_alignment>
    static result<buffer_base> create(size_t size);
    template<typename t_, size_t align_ = default_alignment>
    static result<buffer_base> from(span<t_>);
    template<typename t_, size_t align_ = default_alignment>
    static result<buffer_base> from(span<const t_>);

private:
    [[nodiscard]] const type* _get() const { return m_ptr.get(); }
    [[nodiscard]] type* _get() { return m_ptr.get(); }

    using ptr_type = unique_ptr<type, mem_free_deleter<uint8_t>>;

    ptr_type m_ptr;
    size_t m_size = 0;
};

template<memory_type mem_t_>
buffer_base<mem_t_>::buffer_base(void* ptr, const size_t size)
    : m_ptr(static_cast<pointer>(ptr))
    , m_size(size)
{}

template<memory_type mem_t_>
constexpr buffer_base<mem_t_>::operator bool() const { return m_ptr.has_value(); }

template<memory_type mem_t_>
constexpr size_t buffer_base<mem_t_>::size() const { return m_size; }

template<memory_type mem_t_>
template<typename t_>
const t_* buffer_base<mem_t_>::data(const size_t offset) const {
    if (offset * sizeof(t_) >= m_size) { catastrophic_error("out of buffer range"); }
    return reinterpret_cast<const t_*>(_get()) + offset;
}

template<memory_type mem_t_>
template<typename t_>
t_* buffer_base<mem_t_>::data(const size_t offset) {
    if (offset * sizeof(t_) >= m_size) { catastrophic_error("out of buffer range"); }
    return reinterpret_cast<t_*>(_get()) + offset;
}

template<memory_type mem_t_>
template<typename t_>
span<const t_> buffer_base<mem_t_>::view() const noexcept { return {reinterpret_cast<const t_*>(m_ptr.get()), m_size / sizeof(t_)}; }

template<memory_type mem_t_>
template<typename t_>
span<t_> buffer_base<mem_t_>::view() noexcept { return {reinterpret_cast<t_*>(m_ptr.get()), m_size / sizeof(t_)}; }

template<memory_type mem_t_>
template<typename t_>
span<const t_> buffer_base<mem_t_>::sub_view(size_t offset, size_t size) const noexcept { return view<t_>().sub(offset, size); }

template<memory_type mem_t_>
template<typename t_>
span<t_> buffer_base<mem_t_>::sub_view(size_t offset, size_t size) noexcept { return view<t_>().sub(offset, size); }

template<memory_type mem_t_>
void buffer_base<mem_t_>::fill(const uint8_t b) {
    _ministl_builtin_memset(m_ptr.get(), b, m_size);
}

template<memory_type mem_t_>
void buffer_base<mem_t_>::clear() {
    _ministl_builtin_memset(m_ptr.get(), 0, m_size);
}

template<memory_type mem_t_>
template<size_t align_>
result<> buffer_base<mem_t_>::resize(const size_t new_size) {
    auto* old_ptr = m_ptr.release();
    if (auto result = framework::reallocate(old_ptr, new_size, mem_t_, align_)) {
        m_ptr = static_cast<uint8_t*>(result.release_value());
        m_size = new_size;
        return {};
    } else {
        m_ptr = old_ptr;
        return framework::err(result.release_error());
    }
}

template<memory_type mem_t_>
template<size_t align_>
result<buffer_base<mem_t_>> buffer_base<mem_t_>::copy() const {
    auto new_buff = verify(buffer_base::create<align_>(m_size));
    _ministl_builtin_memcpy(new_buff.data(), _get(), m_size);

    return framework::ok(framework::move(new_buff));
}

template<memory_type mem_t_>
template<size_t align_>
result<buffer_base<mem_t_>> buffer_base<mem_t_>::sub(const size_t offset, const size_t size) const {
    assert(offset + size <= m_size, "out of buffer range");
    auto buffer = verify(buffer_base::from<uint8_t, align_>(sub_view(offset, size)));
    return framework::ok(framework::move(buffer));
}

template<memory_type mem_t_>
template<size_t align_>
result<buffer_base<mem_t_>> buffer_base<mem_t_>::create(const size_t size) {
    auto ptr = verify(framework::allocate(size, mem_t_, align_));
    return framework::ok(buffer_base(ptr, size));
}

template<memory_type mem_t_>
template<typename t_, size_t align_>
result<buffer_base<mem_t_>> buffer_base<mem_t_>::from(span<t_> data) {
    auto new_buff = verify(buffer_base::create<align_>(data.size_bytes()));
    _ministl_builtin_memcpy(new_buff.data(), data.data(), data.size_bytes());

    return framework::ok(framework::move(new_buff));
}

template<memory_type mem_t_>
template<typename t_, size_t align_>
result<buffer_base<mem_t_>> buffer_base<mem_t_>::from(span<const t_> data) {
    auto new_buff = verify(buffer_base::create<align_>(data.size_bytes()));
    _ministl_builtin_memcpy(new_buff.data(), data.data(), data.size_bytes());

    return framework::ok(framework::move(new_buff));
}


using data_buffer = buffer_base<memory_type::data>;
using code_buffer = buffer_base<memory_type::code>;
using buffer = data_buffer;

}
