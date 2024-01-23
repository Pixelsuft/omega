#include <omega/memory_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/omega.h>
#define base ((OMG_Memory*)this)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_memory_sdl2_destroy(OMG_MemorySdl2* this) {
    base->is_allocated = false;
    this->sdl2->SDL_free(this);
    return false;
}

void* omg_memory_sdl2_alloc(OMG_MemorySdl2* this, OMG_MemoryExtra extra) {
#if OMG_DEBUG_MEM
    OMG_MemoryExtra* result = this->sdl2->SDL_malloc(extra.size + sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(result)) {
        if (OMG_ISNOTNULL(extra.func)) {
            _OMG_LOG_ERROR(omg_base, "Failed to allocate ", (uint32_t)extra.size, " bytes of memory");
            _OMG_LOG_ERROR(omg_base, _OMG_MEMORY_ALLOC_INFO(&extra));
        }
        return NULL;
    }
#if OMG_DEBUG_FILL_MEM
    if (OMG_ISNOTNULL(omg_base->std))
        omg_base->std->memset(result, 255, extra.size + sizeof(OMG_MemoryExtra));
#endif
    result->filename = extra.filename;
    result->func = extra.func;
    result->size = extra.size;
    result->line = extra.line;
    result->is_allocated = true;
    base->alloc_size += extra.size;
    // if (OMG_ISNOTNULL(result->filename))
    //     this->sdl2->SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Memory Allocation %s:%i", (char*)result->func, (int)result->line);
    return (void*)((size_t)result + sizeof(OMG_MemoryExtra));
#else
    void* result = this->sdl2->SDL_malloc((size_t)extra);
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to allocate ", (uint32_t)extra, " bytes of memory");
        return NULL;
    }
    return result;
#endif
}

void* omg_memory_sdl2_realloc(OMG_MemorySdl2* this, void* ptr, size_t size) {
#if OMG_DEBUG_MEM
    if (OMG_ISNULL(ptr))
        return NULL;
    OMG_MemoryExtra* real_ptr = (OMG_MemoryExtra*)((size_t)ptr - sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(real_ptr))
        return NULL;
    if (!real_ptr->is_allocated)
        return NULL;
    size_t size_before = real_ptr->size;
    OMG_MemoryExtra* new_ptr = this->sdl2->SDL_realloc(real_ptr, size + sizeof(OMG_MemoryExtra));
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
    void* result = this->sdl2->SDL_realloc(ptr, size);
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to reallocate ", (uint32_t)size, " bytes of memory");
        return NULL;
    }
    return result;
#endif
}

bool omg_memory_sdl2_free(OMG_MemorySdl2* this, void* ptr) {
#if OMG_DEBUG_MEM
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
    // if (OMG_ISNOTNULL(real_ptr->filename))
    //     this->sdl2->SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Memory Free %s:%i", (char*)real_ptr->func, (int)real_ptr->line);
    real_ptr->is_allocated = false;
    OMG_MemoryExtra data = *real_ptr;
    this->sdl2->SDL_free(real_ptr);
    if (base->alloc_size >= data.size)
        base->alloc_size -= data.size;
    return false;
#else
    this->sdl2->SDL_free(ptr);
    return false;
#endif
}

size_t omg_memory_sdl2_get_alloc_count(OMG_MemorySdl2* this) {
    if (OMG_ISNULL(this->sdl2->SDL_GetNumAllocations))
        return base->alloc_count;
    return (size_t)this->sdl2->SDL_GetNumAllocations();
}

OMG_MemorySdl2* omg_memory_sdl2_create(void* omg, OMG_Sdl2* sdl2) {
    OMG_MemorySdl2* this = sdl2->SDL_malloc(sizeof(OMG_MemorySdl2));
    if (OMG_ISNULL(this))
        return NULL;
    OMG_BEGIN_POINTER_CAST();
    omg_memory_init(this);
    base->alloc = omg_memory_sdl2_alloc;
    base->realloc = omg_memory_sdl2_realloc;
    base->free = omg_memory_sdl2_free;
    base->destroy = omg_memory_sdl2_destroy;
    base->get_alloc_count = omg_memory_sdl2_get_alloc_count;
    OMG_END_POINTER_CAST();
    this->sdl2 = sdl2;
    this->omg = omg;
    base->is_allocated = true;
    return this;
}
#endif
