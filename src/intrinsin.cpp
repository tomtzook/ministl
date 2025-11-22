
#include "intrinsin.h"


namespace framework {

int memcmp(const void* s1, const void* s2, size_t size) {
    auto* ptr_s1 = static_cast<const uint8_t*>(s1);
    auto* ptr_s2 = static_cast<const uint8_t*>(s2);
    while (size-- > 0) {
        if (*ptr_s1++ != *ptr_s2++)
            return ptr_s1[-1] < ptr_s2[-1] ? -1 : 1;
    }
    return 0;
}

void* memset(void* dest, const int value, const size_t size) {
    for (int i = 0; i < size; i++) {
        static_cast<uint8_t*>(dest)[i] = value;
    }

    return dest;
}

void* memcpy(void* dest, const void* src, const size_t size) {
    for (int i = 0; i < size; i++) {
        static_cast<uint8_t*>(dest)[i] = static_cast<const uint8_t*>(src)[i];
    }

    return dest;
}

size_t strlen(const char* s) {
    size_t count = 0;
    while (*s) {
        ++count;
        ++(s);
    }

    return count;
}

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

}
