
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

result<void*> allocate(const size_t size, const memory_type type, const size_t alignment, const bool clear) {
    const auto total_size = size + sizeof(allocation_header_t) + alignment - 1;

    void* mem = nullptr;
    if (clear) {
        verify_status(heap::calloc(type, total_size, mem));
    } else {
        verify_status(heap::malloc(type, total_size, mem));
    }

    void* aligned_mem;
    if (0 != alignment) {
        aligned_mem = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(mem) +
                sizeof(allocation_header_t)  + alignment - 1) & ~static_cast<uintptr_t>(alignment - 1));
    } else {
        aligned_mem = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(mem) + sizeof(allocation_header_t));
    }

    const auto header = reinterpret_cast<allocation_header_t*>(reinterpret_cast<uintptr_t>(aligned_mem) - sizeof(allocation_header_t));
    header->ptr = reinterpret_cast<uintptr_t>(mem);
    header->type = type;

    return framework::ok(aligned_mem);
}

void free(const void* ptr) {
    const auto header = reinterpret_cast<allocation_header_t*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(allocation_header_t));
    trace_status("free heap failed", heap::free(header->type, reinterpret_cast<const void*>(header->ptr)));
}

}
