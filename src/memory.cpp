
#include "../include/heap.h"
#include "verify.h"

#include "memory.h"

namespace framework {

#pragma pack(push, 1)
struct allocation_header_t {
    uintptr_t ptr;
    memory_type type;
};
#pragma pack(pop)

#define total_alloc_size(_size, _align) ((_size) + sizeof(allocation_header_t) + (_align) - 1)

static void* finish_alloc(void* ptr, const memory_type type, const size_t alignment) {
    void* aligned_mem;
    if (0 != alignment) {
        aligned_mem = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(ptr) +
                sizeof(allocation_header_t)  + alignment - 1) & ~static_cast<uintptr_t>(alignment - 1));
    } else {
        aligned_mem = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) + sizeof(allocation_header_t));
    }

    const auto header = reinterpret_cast<allocation_header_t*>(reinterpret_cast<uintptr_t>(aligned_mem) - sizeof(allocation_header_t));
    header->ptr = reinterpret_cast<uintptr_t>(ptr);
    header->type = type;

    return aligned_mem;
}

static void* original_ptr(void* ptr) {
    const auto header = reinterpret_cast<allocation_header_t*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(allocation_header_t));
    return reinterpret_cast<void*>(header->ptr);
}

result<void*> allocate(const size_t size, const memory_type type, const size_t alignment, const bool set, const uint8_t initial_value) {
    const auto total_size = total_alloc_size(size, alignment);

    void* mem = nullptr;
    if (set) {
        verify_status(heap::calloc(type, initial_value, total_size, mem));
    } else {
        verify_status(heap::malloc(type, total_size, mem));
    }

    void* aligned_mem = finish_alloc(mem, type, alignment);
    return framework::ok(aligned_mem);
}

result<void*> reallocate(void* ptr, const size_t new_size, const memory_type type, const size_t alignment) {
    const auto total_size = total_alloc_size(new_size, alignment);

    void* actual_ptr = original_ptr(ptr);
    void* mem = nullptr;
    verify_status(heap::realloc(type, actual_ptr, total_size, mem));

    void* aligned_mem = finish_alloc(mem, type, alignment);
    return framework::ok(aligned_mem);
}

void free(const void* ptr) {
    const auto header = reinterpret_cast<allocation_header_t*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(allocation_header_t));
    trace_status("free heap failed", heap::free(header->type, reinterpret_cast<const void*>(header->ptr)));
}

}
