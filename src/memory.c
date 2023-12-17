#include <omega/memory.h>

bool omg_memory_destroy(OMG_Memory* this) {
    OMG_UNUSED(this);
    return false;
}

void* omg_memory_alloc(OMG_Memory* this, OMG_MemoryExtra extra) {
    OMG_UNUSED(this, extra);
    return NULL;
}

void* omg_memory_realloc(OMG_Memory* this, void* ptr, size_t size) {
    OMG_UNUSED(this, ptr, size);
    return NULL;
}

bool omg_memory_free(OMG_Memory* this, void* ptr) {
    OMG_UNUSED(this, ptr);
    return false;
}

size_t omg_memory_get_alloc_count(OMG_Memory* this) {
    return this->alloc_count;
}

bool omg_memory_init(OMG_Memory* this) {
    this->alloc_count = this->alloc_size = 0;
    this->is_allocated = false;
    this->destroy = omg_memory_destroy;
    this->alloc = omg_memory_alloc;
    this->realloc = omg_memory_realloc;
    this->free = omg_memory_free;
    this->get_alloc_count = omg_memory_get_alloc_count;
    return false;
}
