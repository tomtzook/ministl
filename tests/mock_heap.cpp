
#include <cstddef>
#include <cstdint>
#include <heap.h>

namespace framework::heap {

status malloc(memory_type, size_t, void*&) {
    return status_unimplemented;
}

status realloc(memory_type, void*, size_t, void*&) {
    return status_unimplemented;
}

status calloc(memory_type, size_t, void*&) {
    return status_unimplemented;
}

status free(memory_type, const void*) {
    return status_unimplemented;
}

}
