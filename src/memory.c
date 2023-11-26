#include <omega/memory.h>

bool omg_memory_destroy(OMG_Memory* this) {
    OMG_UNUSED(this);
    return false;
}

void* omg_memory_alloc(OMG_Memory* this, OMG_MemoryExtra extra) {
    OMG_UNUSED(this, extra);
    return NULL;
}

bool omg_memory_free(OMG_Memory* this, void* ptr) {
    OMG_UNUSED(this, ptr);
    return false;
}

bool omg_memory_init(OMG_Memory* this) {
    this->alloc_count = this->alloc_size = 0;
    this->is_allocated = false;
    this->destroy = omg_memory_destroy;
    this->alloc = omg_memory_alloc;
    this->free = omg_memory_free;
    return false;
}
