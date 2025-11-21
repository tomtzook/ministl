
#include "../include/heap.h"

namespace framework::heap {

heap::heap()
    : m_head(nullptr)
    , m_next_block_ptr(0)
    , m_remaining_mem(0)
{}

status heap::init(void* memory_region, const size_t region_size) {
    m_head = nullptr;
    m_next_block_ptr = reinterpret_cast<uintptr_t>(memory_region);
    m_remaining_mem = region_size;

    if (create_new_block(min_block_size) == nullptr) {
        return status_no_space;
    }

    return {};
}

status heap::malloc(const size_t size, void*& out_ptr) {
    auto* block = find_block_for_size(size);
    if (block == nullptr) {
        // no block found
        block = create_new_block(size);
        if (block == nullptr) {
            return status_no_space;
        }
    }

    block->in_use = true;
    out_ptr = block->data;
    return {};
}

status heap::free(const void* ptr) {
    auto* block = get_block_from_ptr(const_cast<void*>(ptr));
    if (block == nullptr) {
        return status_bad_arg;
    }

    block->in_use = false;
    return {};
}

heap::block* heap::create_new_block(const size_t min_size) {
    const auto block_size =
        ((min_size < min_block_size ? min_block_size : min_size) + block_alignment - 1) & ~(block_alignment - 1);
    const auto total_size =
        sizeof(block) + block_size + block_alignment - 1;

    auto* mem = request_new_memory(total_size);
    if (mem == nullptr) {
        return nullptr;
    }

    mem = reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(mem) + block_alignment - 1) & ~static_cast<uintptr_t>(block_alignment - 1));
    auto* block_mem = static_cast<block*>(mem);
    block_mem->magic = block_magic;
    block_mem->next = nullptr;
    block_mem->size = block_size;
    block_mem->in_use = false;

    insert_block(block_mem);

    return block_mem;
}

void* heap::request_new_memory(const size_t size) {
    if (size > m_remaining_mem) {
        return nullptr;
    }

    m_remaining_mem -= size;
    const auto next_ptr = m_next_block_ptr;
    m_next_block_ptr += size;

    return reinterpret_cast<void*>(next_ptr);
}

void heap::insert_block(block* new_block) {
    if (m_head == nullptr) {
        m_head = new_block;
        return;
    }

    auto block = m_head;
    for (; block->next != nullptr; block = block->next) {}
    block->next = new_block;
}

heap::block* heap::get_block_from_ptr(void* ptr) {
    constexpr auto offset = __builtin_offsetof(block, data);
    auto* blk = reinterpret_cast<block*>(reinterpret_cast<uintptr_t>(ptr) - offset);
    if (blk->magic != block_magic) {
        return nullptr;
    }

    return blk;
}

heap::block* heap::find_block_for_size(const size_t size) const {
    for (auto block = m_head; block != nullptr; block = block->next) {
        if (block->in_use || block->size < size || (block->size - size) > min_block_size) {
            continue;
        }

        // we have an available block of memory
        return block;
    }

    return nullptr;
}

}
