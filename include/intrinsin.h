#pragma once

#ifdef __clang__

#define _ministl_builtin_memcpy(_dest, _src, _size) __builtin_memcpy((_dest), (_src), (_size))
#define _ministl_builtin_memset(_dest, _val, _size) __builtin_memset((_dest), (_val), (_size))

#define _ministl_builtin_atomic_load(_ptr, _order) __atomic_load_n((_ptr), (_order))
#define _ministl_builtin_atomic_store(_ptr, _val, _order) __atomic_store_n((_ptr), (_val), (_order))
#define _ministl_builtin_atomic_exchange(_ptr, _val, _order) __atomic_exchange_n((_ptr), (_val), (_order))
#define _ministl_builtin_atomic_cmpxchg(_ptr, _expect_ptr, _desired, _order_success, _order_failure) __atomic_compare_exchange_n((_ptr), (_expect_ptr), (_desired), 0, (_order_success), (_order_failure))
#define _ministl_builtin_atomic_fetch_add(_ptr, _val, _order) __atomic_fetch_add((_ptr), (_val), (_order))
#define _ministl_builtin_atomic_add_fetch(_ptr, _val, _order) __atomic_add_fetch((_ptr), (_val), (_order))
#define _ministl_builtin_atomic_fetch_sub(_ptr, _val, _order) __atomic_fetch_sub((_ptr), (_val), (_order))
#define _ministl_builtin_atomic_sub_fetch(_ptr, _val, _order) __atomic_sub_fetch((_ptr), (_val), (_order))

#define _ministl_builtin_atomic_order_relaxed __ATOMIC_RELAXED
#define _ministl_builtin_atomic_order_consume __ATOMIC_CONSUME
#define _ministl_builtin_atomic_order_acquire __ATOMIC_ACQUIRE
#define _ministl_builtin_atomic_order_release __ATOMIC_RELEASE
#define _ministl_builtin_atomic_order_acq_rel __ATOMIC_ACQ_REL
#define _ministl_builtin_atomic_order_seq_cst __ATOMIC_SEQ_CST

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
