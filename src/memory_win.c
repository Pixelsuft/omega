#include <omega/config.h>

// TODO: show warn on errors

#if OMG_SUPPORT_WIN
#include <omega/winapi.h>
#include <omega/memory_win.h>
#if OMG_DEBUG
#include <omega/omega.h>
#include <omega/omega_win.h>
#endif
#define base ((OMG_Memory*)this)

bool omg_memory_win_destroy(OMG_MemoryWin* this) {
    bool result = false;
    HANDLE heap = this->heap;
    OMG_Kernel32* k32 = this->k32;
    if (!k32->HeapFree(heap, 0, this))
        result = true;
    if (!k32->HeapDestroy(heap))
        result = true;
    return result;
}

void* omg_memory_win_alloc(OMG_MemoryWin* this, OMG_MemoryExtra extra) {
#if OMG_DEBUG
    OMG_MemoryExtra* result = this->k32->HeapAlloc(this->heap, 0, extra.size + sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(result)) {
        OMG_Omega* omg = omg_get_default_omega();
        if (OMG_ISNOTNULL(omg) && OMG_ISNOTNULL(extra.func)) {
            DWORD error = ((OMG_OmegaWin*)omg)->k32->GetLastError();
            // For the future
            /*wchar_t* w_error_buffer = _OMG_INTERNAL_MALLOC((OMG_Memory*)this, (128 + 64) * 1024);
            char* error_buffer = (char*)((size_t)w_error_buffer + (128 * 1024));
            if (OMG_ISNOTNULL(error_buffer)) {
                if (((OMG_OmegaWin*)omg)->k32->FormatMessageW(
                    OMG_WIN_FORMAT_MESSAGE_FROM_SYSTEM | OMG_WIN_FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL, error, 0, w_error_buffer, 64 * 1024, NULL
                )) {}
                OMG_FREE((OMG_Memory*)this, w_error_buffer);
            }*/
            _OMG_LOG_ERROR(omg, "Failed to allocate ", (uint32_t)extra.size, " bytes of memory (Error Code - ", error, ")");
        }
        return NULL;
    }
    result->filename = extra.filename;
    result->func = extra.func;
    result->size = extra.size;
    result->line = extra.line;
    result->is_allocated = true;
    base->alloc_count++;
    base->alloc_size += extra.size;
    return (void*)((size_t)result + sizeof(OMG_MemoryExtra));
#else
    OMG_UNUSED(extra);
    void* result = this->k32->HeapAlloc(this->heap, 0, (size_t)extra);
    if (OMG_ISNULL(result)) {
        OMG_Omega* omg = omg_get_default_omega();
        if (OMG_ISNOTNULL(omg) && OMG_ISNOTNULL(extra.func)) {
            _OMG_LOG_ERROR(omg, "Failed to allocate ", (uint32_t)extra.size, " bytes of memory");
        }
        return NULL;
    }
    return result;
#endif
}

void* omg_memory_win_realloc(OMG_MemoryWin* this, void* ptr, size_t size) {
#if OMG_DEBUG
    if (OMG_ISNULL(ptr))
        return NULL;
    OMG_MemoryExtra* real_ptr = (OMG_MemoryExtra*)((size_t)ptr - sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(real_ptr))
        return NULL;
    if (!real_ptr->is_allocated)
        return NULL;
    size_t size_before = real_ptr->size;
    OMG_MemoryExtra* new_ptr = this->k32->HeapReAlloc(this->heap, 0, real_ptr, size + sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(new_ptr)) {
        if (base->alloc_size >= size_before)
            base->alloc_size -= size_before;
        if (base->alloc_count > 0)
            base->alloc_count--;
        return NULL;
    }
    base->alloc_size = base->alloc_size + size - size_before;
    new_ptr->size = size;
    return (void*)((size_t)new_ptr + sizeof(OMG_MemoryExtra));
#else
    void* result = this->k32->HeapReAlloc(this->heap, 0, ptr, size);
    if (OMG_ISNULL(result)) {
        return NULL;
    }
    return result;
#endif
}

bool omg_memory_win_free(OMG_MemoryWin* this, void* ptr) {
#if OMG_DEBUG
    if (OMG_ISNULL(ptr))
        return true;
    OMG_MemoryExtra* real_ptr = (OMG_MemoryExtra*)((size_t)ptr - sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(real_ptr))
        return true;
    if (!real_ptr->is_allocated)
        return true;
    real_ptr->is_allocated = false;
    OMG_MemoryExtra data = *real_ptr;
    if (this->k32->HeapFree(this->heap, 0, real_ptr)) {
        if (base->alloc_size >= data.size)
            base->alloc_size -= data.size;
        if (base->alloc_count > 0)
            base->alloc_count--;
        return false;
    }
    return true;
#else
    return !this->k32->HeapFree(this->heap, 0, ptr);
#endif
}

OMG_MemoryWin* omg_memory_win_create(OMG_Kernel32* data) {
    // TODO: function, that doesn't allocate object (omg_memory_win_init)
    HANDLE heap = data->HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
    if (heap == NULL)
        return NULL;
    OMG_MemoryWin* this = data->HeapAlloc(heap, 0, sizeof(OMG_MemoryWin));
    if (this == NULL) {
        data->HeapDestroy(heap);
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_memory_init(this);
    base->alloc = omg_memory_win_alloc;
    base->realloc = omg_memory_win_realloc;
    base->free = omg_memory_win_free;
    base->destroy = omg_memory_win_destroy;
    OMG_END_POINTER_CAST();
    this->k32 = data;
    this->heap = heap;
    base->alloc_count = 0;
    base->is_allocated = true;
    return this;
}
#endif
