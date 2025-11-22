#pragma once

#ifdef _ministl_define_types_

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

#else

#include "stdint.h" // NOLINT(*-deprecated-headers)
#include "stddef.h" // NOLINT(*-deprecated-headers)

#endif


#if _ministl_size_t_type_ == 1

typedef uint32_t size_t;
typedef int32_t ssize_t;

#elif _ministl_size_t_type_ == 2

typedef uint64_t size_t;
typedef int64_t ssize_t;

#endif
