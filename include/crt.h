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

#ifdef _ministl_define_intrinsic_

extern "C"
int memcmp(const void*, const void*, size_t);
extern "C"
void* memset(void*, int, size_t);
extern "C"
void* memcpy(void*, const void*, size_t);
extern "C"
size_t strlen(const char* s);
extern "C"
int strcmp(const char* s1, const char* s2);

#endif
