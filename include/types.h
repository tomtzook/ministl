#pragma once

#if _ministl_define_types_ == 1

#ifdef __x86_64__

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;
typedef int64_t intn_t;
typedef int64_t intptr_t;
typedef long int intmax_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint64_t uintn_t;
typedef uint64_t uintptr_t;
typedef unsigned long int uintmax_t;

#else
#define "undefined architecture"
#endif

#elif _ministl_define_types_ == 2

#include "stdint.h" // NOLINT(*-deprecated-headers)
#include "stddef.h" // NOLINT(*-deprecated-headers)

#endif


#if _ministl_size_t_type_ == 1
typedef unsigned long size_t;
typedef long ssize_t;
#elif _ministl_size_t_type_ == 2
typedef unsigned long long size_t;
typedef long long ssize_t;
#endif
