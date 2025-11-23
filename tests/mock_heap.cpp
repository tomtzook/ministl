
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <heap.h>

namespace framework::heap {

status malloc(memory_type, const size_t size, void*& out_ptr) {
    auto* ptr = ::malloc(size);
    if (!ptr) {
        return framework::status_bad_alloc;
    }

    out_ptr = ptr;
    return {};
}

status realloc(memory_type, void* old_ptr, const size_t new_size, void*& out_ptr) {
    auto* new_ptr = ::realloc(old_ptr, new_size);
    if (!new_ptr) {
        return framework::status_bad_alloc;
    }

    out_ptr = new_ptr;
    return {};
}

status calloc(memory_type, const uint8_t memb, const size_t size, void*& out_ptr) {
    auto* ptr = ::calloc(memb, size);
    if (!ptr) {
        return framework::status_bad_alloc;
    }

    out_ptr = ptr;
    return {};
}

status free(memory_type, const void* ptr) {
    ::free(const_cast<void*>(ptr));
    return {};
}

}
