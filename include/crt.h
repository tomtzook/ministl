#pragma once

#include "types.h"

namespace std {
enum align_val_t: size_t {}; // NOLINT(*-dcl58-cpp)
}

void* operator new(size_t, size_t) noexcept;

inline void* operator new(size_t, void* ptr) noexcept { return ptr; }
inline void operator delete(void*, void*) noexcept {}
