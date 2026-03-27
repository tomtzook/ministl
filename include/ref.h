#pragma once

namespace framework {

template<typename t_>
struct reference_wrapper {
    using type = t_;

    // ReSharper disable once CppNonExplicitConvertingConstructor
    reference_wrapper(type& data) : m_data(&data) {}

    reference_wrapper(const reference_wrapper&) = default;
    reference_wrapper(reference_wrapper&&) = default;

    reference_wrapper& operator=(const reference_wrapper&) = default;
    reference_wrapper& operator=(reference_wrapper&&) = default;

    // ReSharper disable once CppNonExplicitConversionOperator
    operator type&() const noexcept { return *m_data; }
    type& get() const { return *m_data; }

private:
    type* m_data;
};

}