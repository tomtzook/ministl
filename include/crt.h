#pragma once

#include "types.h"

#ifndef _NEW
namespace std {
enum align_val_t: size_t {}; // NOLINT(*-dcl58-cpp)
}
#endif

void* operator new(size_t, size_t) noexcept;

#ifndef _NEW
inline void* operator new(size_t, void* ptr) noexcept { return ptr; }
inline void operator delete(void*, void*) noexcept {}
#endif
