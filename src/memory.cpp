
#include "memory.h"

namespace framework {

result<void*> allocate(size_t size, memory_type type, size_t alignment) {
    return framework::err(framework::status_unsupported);
}

void free(const void* ptr) {

}

}
