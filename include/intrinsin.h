#pragma once

#ifdef __clang__

#define _ministl_builtin_memcpy(_dest, _src, _size) __builtin_memcpy((_dest), (_src), (_size))
#define _ministl_builtin_memset(_dest, _val, _size) __builtin_memset((_dest), (_val), (_size))

#ifdef __x86_64__
#define _ministl_builtin_pause() __builtin_ia32_pause()
#else
#error "unsupported architecture"
#endif

#else
#error "unsupported compiler"
#endif

#include "types.h"

namespace framework {

int memcmp(const void* s1, const void* s2, size_t size);
void* memset(void* dest, int value, size_t size);
void* memcpy(void* dest, const void* src, size_t size);
size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);

}
