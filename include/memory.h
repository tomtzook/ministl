#pragma once

#include "result.h"

namespace framework {

enum class memory_type {
    data,
    code
};

static constexpr size_t default_alignment = 16;

result<void*> allocate(size_t size, memory_type type = memory_type::data, size_t alignment = default_alignment, bool set = false, uint8_t initial_value = 0);
result<void*> reallocate(void* ptr, size_t new_size, memory_type type = memory_type::data, size_t alignment = default_alignment);
void free(const void* ptr);

}
