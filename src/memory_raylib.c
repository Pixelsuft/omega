#include <omega/memory_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#define base ((OMG_Memory*)this)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_memory_raylib_destroy(OMG_MemoryRaylib* this) {
    base->is_allocated = false;
    this->raylib->MemFree(this);
    return false;
}

void* omg_memory_raylib_alloc(OMG_MemoryRaylib* this, OMG_MemoryExtra extra) {
#if OMG_DEBUG
    OMG_MemoryExtra* result = this->raylib->MemAlloc(extra.size + sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(result)) {
        if (OMG_ISNOTNULL(extra.func)) {
            _OMG_LOG_ERROR(omg_base, "Failed to allocate ", (uint32_t)extra.size, " bytes of memory");
            _OMG_LOG_ERROR(omg_base, _OMG_MEMORY_ALLOC_INFO(&extra));
        }
        return NULL;
    }
    result->filename = extra.filename;
    result->func = extra.func;
    result->size = extra.size;
    result->line = extra.line;
    result->is_allocated = true;
    base->alloc_size += extra.size;
    base->alloc_count += 1;
    return (void*)((size_t)result + sizeof(OMG_MemoryExtra));
#else
    void* result = this->raylib->MemAlloc((size_t)extra);
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to allocate ", (uint32_t)extra, " bytes of memory");
        return NULL;
    }
    return result;
#endif
}

void* omg_memory_raylib_realloc(OMG_MemoryRaylib* this, void* ptr, size_t size) {
#if OMG_DEBUG
    if (OMG_ISNULL(ptr))
        return NULL;
    OMG_MemoryExtra* real_ptr = (OMG_MemoryExtra*)((size_t)ptr - sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(real_ptr))
        return NULL;
    if (!real_ptr->is_allocated)
        return NULL;
    size_t size_before = real_ptr->size;
    OMG_MemoryExtra* new_ptr = this->raylib->MemRealloc(real_ptr, size + sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(new_ptr)) {
        if (OMG_ISNOTNULL(real_ptr->func)) {
            _OMG_LOG_ERROR(omg_base, "Failed to reallocate ", (uint32_t)real_ptr->size, " bytes of memory");
            _OMG_LOG_ERROR(omg_base, _OMG_MEMORY_ALLOC_INFO(real_ptr));
        }
        return NULL;
    }
    base->alloc_size = base->alloc_size + size - size_before;
    new_ptr->size = size;
    return (void*)((size_t)new_ptr + sizeof(OMG_MemoryExtra));
#else
    void* result = this->raylib->MemRealloc(ptr, size);
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to reallocate ", (uint32_t)size, " bytes of memory");
        return NULL;
    }
    return result;
#endif
}

bool omg_memory_raylib_free(OMG_MemoryRaylib* this, void* ptr) {
#if OMG_DEBUG
    if (OMG_ISNULL(ptr)) {
        _OMG_LOG_WARN(omg_base, _OMG_MEMORY_NULL_FREE_WARN);
        return true;
    }
    OMG_MemoryExtra* real_ptr = (OMG_MemoryExtra*)((size_t)ptr - sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(real_ptr)) {
        _OMG_LOG_WARN(omg_base, _OMG_MEMORY_NULL_FREE_WARN);
        return true;
    }
    if (!real_ptr->is_allocated) {
        _OMG_LOG_WARN(omg_base, _OMG_MEMORY_NULL_FREE_WARN);
        return true;
    }
    real_ptr->is_allocated = false;
    OMG_MemoryExtra data = *real_ptr;
    this->raylib->MemFree(real_ptr);
    if (base->alloc_size >= data.size)
        base->alloc_size -= data.size;
    return false;
#else
    this->raylib->MemFree(ptr);
    return false;
#endif
}

OMG_MemoryRaylib* omg_memory_raylib_create(void* omg, OMG_Raylib* raylib) {
    OMG_MemoryRaylib* this = raylib->MemAlloc(sizeof(OMG_MemoryRaylib));
    if (OMG_ISNULL(this))
        return NULL;
    OMG_BEGIN_POINTER_CAST();
    omg_memory_init(this);
    base->alloc = omg_memory_raylib_alloc;
    base->realloc = omg_memory_raylib_realloc;
    base->free = omg_memory_raylib_free;
    base->destroy = omg_memory_raylib_destroy;
    OMG_END_POINTER_CAST();
    this->raylib = raylib;
    this->omg = omg;
    base->is_allocated = true;
    return this;
}
#endif