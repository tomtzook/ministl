
#include "memory.h"
#include "crt.h"

#include "intrinsin.h"

#ifdef _ministl_define_crt_

void* operator new(const size_t size) {
    return operator new(size, framework::default_alignment);
}

void* operator new(const size_t size, std::align_val_t) {
    return operator new(size);
}

void* operator new(const size_t size, const size_t alignment) noexcept {
    auto ptr_result = framework::allocate(size, framework::memory_type::data, alignment);
    if (ptr_result) {
        return ptr_result.release_value();
    }

    return nullptr;
}

void* operator new[](const size_t size) {
    return operator new(size);
}

void* operator new[](const size_t size, std::align_val_t) {
    return operator new(size);
}

void* operator new[](const size_t size, const size_t alignment) noexcept {
    return operator new(size, alignment);
}

// ReSharper disable once CppParameterMayBeConstPtrOrRef
void operator delete(void* memory) noexcept {
    framework::free(memory);
}

void operator delete(void* memory, std::align_val_t) noexcept {
    operator delete(memory);
}

void operator delete[](void* memory) noexcept {
    operator delete(memory);
}

void operator delete[](void* memory, std::align_val_t) noexcept {
    operator delete(memory);
}

#endif

#ifdef _ministl_define_intrinsic_

#include "intrinsin.h"

extern "C"
int memcmp(const void* s1, const void* s2, size_t size) {
    return framework::memcmp(s1, s2, size);
}

extern "C"
void* memset(void* dest, const int value, const size_t size) {
    return framework::memset(dest, value, size);
}

extern "C"
void* memcpy(void* dest, const void* src, const size_t size) {
    return framework::memcpy(dest, src, size);
}

extern "C"
size_t strlen(const char* s) {
    return framework::strlen(s);
}

extern "C"
int strcmp(const char* s1, const char* s2) {
    return framework::strcmp(s1, s2);
}

#endif
