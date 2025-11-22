
#include "memory.h"
#include "crt.h"

#ifdef __define_crt__

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

#ifdef __define_intrinsic__

extern "C"
int memcmp(const void* s1, const void* s2, size_t size) {
    auto* ptr_s1 = static_cast<const uint8_t*>(s1);
    auto* ptr_s2 = static_cast<const uint8_t*>(s2);
    while (size-- > 0) {
        if (*ptr_s1++ != *ptr_s2++)
            return ptr_s1[-1] < ptr_s2[-1] ? -1 : 1;
    }
    return 0;
}

extern "C"
void* memset(void* dest, const int value, const size_t size) {
    for (int i = 0; i < size; i++) {
        static_cast<uint8_t*>(dest)[i] = value;
    }

    return dest;
}

extern "C"
void* memcpy(void* dest, const void* src, const size_t size) {
    for (int i = 0; i < size; i++) {
        static_cast<uint8_t*>(dest)[i] = static_cast<const uint8_t*>(src)[i];
    }

    return dest;
}

extern "C"
size_t strlen(const char* s) {
    size_t count = 0;
    while (*s) {
        ++count;
        ++(s);
    }

    return count;
}

extern "C"
int strcmp(const char* s1, const char* s2) {
    while (*s1) {
        if (*s1 != *s2) {
            break;
        }
        ++(s1);
        ++(s2);
    }
    return *reinterpret_cast<const unsigned char*>(s1) - *reinterpret_cast<const unsigned char*>(s2);
}

#endif
