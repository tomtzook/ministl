#pragma once

#include "types.h"
#include "status.h"
#include "memory.h"

namespace framework::heap {

class heap {
public:
    heap();
    heap(const heap&) = delete;
    heap(heap&&) = delete;
    ~heap() = default;

    heap& operator=(const heap&) = delete;
    heap& operator=(heap&&) = delete;

    [[nodiscard]] status init(void* memory_region, size_t region_size);
    [[nodiscard]] status malloc(size_t size, void*& out_ptr);
    [[nodiscard]] status realloc(void* old_ptr, size_t new_size, void*& out_ptr);
    [[nodiscard]] status calloc(size_t size, void*& out_ptr);
    status free(const void* ptr);

private:
    static constexpr size_t min_block_size = 32;
    static constexpr size_t block_alignment = 32;
    static constexpr uint16_t block_magic = 0xabab;
    struct block {
        uint16_t magic;
        block* next;
        size_t size;
        bool in_use;
        alignas(block_alignment) uint8_t data[];
    };

    [[nodiscard]] status malloc_ex(size_t size, bool clear, void*& out_ptr);
    void free_block(block* block);
    [[nodiscard]] block* create_new_block(size_t min_size);
    [[nodiscard]] void* request_new_memory(size_t size);
    void insert_block(block* new_block);
    [[nodiscard]] block* get_block_from_ptr(void* ptr);
    [[nodiscard]] block* find_block_for_size(size_t size) const;

    block* m_head;
    uintptr_t m_next_block_ptr;
    size_t m_remaining_mem;
};

[[nodiscard]] extern status malloc(memory_type type, size_t size, void*& out_ptr);
[[nodiscard]] extern status realloc(memory_type type, void* old_ptr, size_t new_size, void*& out_ptr);
[[nodiscard]] extern status calloc(memory_type type, size_t size, void*& out_ptr);
extern status free(memory_type type, const void* ptr);

}
