#include <omega/config.h>
#if OMG_SUPPORT_WIN
#include <omega/api_win.h>
#include <omega/memory_win.h>
#include <omega/omega.h>
#if OMG_DEBUG_MEM
#include <omega/omega.h>
#include <omega/omega_win.h>
#endif
#define base ((OMG_Memory*)this)
#define omg_base ((OMG_Omega*)this->omg)
#define omg_win ((OMG_OmegaWin*)this->omg)

bool omg_memory_win_destroy(OMG_MemoryWin* this) {
    bool result = false;
    HANDLE heap = this->heap;
    OMG_Kernel32* k32 = this->k32;
    base->is_allocated = false;
    if (!k32->HeapFree(heap, 0, this))
        result = true;
    if (!k32->HeapDestroy(heap))
        result = true;
    return result;
}

void* omg_memory_win_alloc(OMG_MemoryWin* this, OMG_MemoryExtra extra) {
#if OMG_DEBUG_MEM
    OMG_MemoryExtra* result = ((OMG_Kernel32*)omg_base->k32)->HeapAlloc(this->heap, 0, extra.size + sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(result)) {
        if (omg_base->type == OMG_OMEGA_TYPE_WIN && OMG_ISNOTNULL(extra.func)) {
            DWORD error = this->k32->GetLastError();
            DWORD res;
            wchar_t* w_error_buffer;
            _OMG_WIN_FORMAT_ERROR((OMG_Memory*)this, this->k32, error, w_error_buffer, res);
            if (res > 2) {
                w_error_buffer[res - 3] = L'\0';
                _OMG_LOG_ERROR(omg_base, "Failed to allocate ", (uint32_t)extra.size, " bytes of memory (", w_error_buffer, ")");
            }
            else
                _OMG_LOG_ERROR(omg_base, "Failed to allocate ", (uint32_t)extra.size, " bytes of memory (Error Code - ", error, ")");
            if (OMG_ISNOTNULL(w_error_buffer))
                OMG_FREE((OMG_Memory*)this, w_error_buffer);
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
    base->alloc_count++;
    base->alloc_size += extra.size;
    // if (OMG_ISNOTNULL(result->func))
    //     printf("%s:%i alloc\n", result->func, (int)result->line);
    return (void*)((size_t)result + sizeof(OMG_MemoryExtra));
#else
    void* result = this->k32->HeapAlloc(this->heap, 0, (size_t)extra);
    if (OMG_ISNULL(result)) {
        DWORD error = this->k32->GetLastError();
        _OMG_LOG_ERROR(omg_base, "Failed to allocate ", (uint32_t)extra, " bytes of memory (Error Code - ", error, ")");
        return NULL;
    }
    return result;
#endif
}

void* omg_memory_win_realloc(OMG_MemoryWin* this, void* ptr, size_t size) {
#if OMG_DEBUG_MEM
    if (OMG_ISNULL(ptr))
        return NULL;
    OMG_MemoryExtra* real_ptr = (OMG_MemoryExtra*)((size_t)ptr - sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(real_ptr))
        return NULL;
    if (!real_ptr->is_allocated)
        return NULL;
    size_t size_before = real_ptr->size;
    OMG_MemoryExtra old_data = *real_ptr;
    OMG_MemoryExtra* new_ptr = this->k32->HeapReAlloc(this->heap, 0, real_ptr, size + sizeof(OMG_MemoryExtra));
    if (OMG_ISNULL(new_ptr)) {
        if (omg_base->type == OMG_OMEGA_TYPE_WIN && OMG_ISNOTNULL(real_ptr->func)) {
            DWORD error = this->k32->GetLastError();
            DWORD res;
            wchar_t* w_error_buffer;
            _OMG_WIN_FORMAT_ERROR((OMG_Memory*)this, this->k32, error, w_error_buffer, res);
            if (res > 2) {
                w_error_buffer[res - 3] = L'\0';
                _OMG_LOG_ERROR(omg_base, "Failed to reallocate ", (uint32_t)real_ptr->size, " bytes of memory (", w_error_buffer, ")");
            }
            else
                _OMG_LOG_ERROR(omg_base, "Failed to reallocate ", (uint32_t)real_ptr->size, " bytes of memory (Error Code - ", error, ")");
            if (OMG_ISNOTNULL(w_error_buffer))
                OMG_FREE((OMG_Memory*)this, w_error_buffer);
            _OMG_LOG_ERROR(omg_base, _OMG_MEMORY_ALLOC_INFO(real_ptr));
        }
        return NULL;
    }
    if (new_ptr != real_ptr) {
        new_ptr->filename = old_data.filename;
        new_ptr->func = old_data.func;
        new_ptr->is_allocated = old_data.is_allocated;
        new_ptr->line = old_data.line;
        new_ptr->size = old_data.size;
    }
    base->alloc_size = base->alloc_size + size - size_before;
    new_ptr->size = size;
    return (void*)((size_t)new_ptr + sizeof(OMG_MemoryExtra));
#else
    void* result = this->k32->HeapReAlloc(this->heap, 0, ptr, size);
    if (OMG_ISNULL(result)) {
        DWORD error = this->k32->GetLastError();
        _OMG_LOG_ERROR(omg_base, "Failed to reallocate ", (uint32_t)size, " bytes of memory (Error Code - ", error, ")");
        return NULL;
    }
    return result;
#endif
}

bool omg_memory_win_free(OMG_MemoryWin* this, void* ptr) {
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
    // if (OMG_ISNOTNULL(real_ptr->func))
    //     printf("%s:%i free\n", real_ptr->func, (int)real_ptr->line);
    real_ptr->is_allocated = false;
    OMG_MemoryExtra data = *real_ptr;
    if (this->k32->HeapFree(this->heap, 0, real_ptr)) {
        if (base->alloc_size >= data.size)
            base->alloc_size -= data.size;
        if (base->alloc_count > 0)
            base->alloc_count--;
        return false;
    }
    if (omg_base->type == OMG_OMEGA_TYPE_WIN && OMG_ISNOTNULL(data.func)) {
        DWORD error = this->k32->GetLastError();
        DWORD res;
        wchar_t* w_error_buffer;
        _OMG_WIN_FORMAT_ERROR((OMG_Memory*)this, this->k32, error, w_error_buffer, res);
        if (res > 2) {
            w_error_buffer[res - 3] = L'\0';
            _OMG_LOG_ERROR(omg_base, "Failed to free ", (uint32_t)data.size, " bytes of memory (", w_error_buffer, ")");
        }
        else
            _OMG_LOG_ERROR(omg_base, "Failed to free ", (uint32_t)data.size, " bytes of memory (Error Code - ", error, ")");
        if (OMG_ISNOTNULL(w_error_buffer))
            OMG_FREE((OMG_Memory*)this, w_error_buffer);
        _OMG_LOG_ERROR(omg_base, _OMG_MEMORY_ALLOC_INFO(&data));
    }
    return true;
#else
    DWORD res = this->k32->HeapFree(this->heap, 0, ptr);
    if (!res) {
        DWORD error = this->k32->GetLastError();
        _OMG_LOG_ERROR(omg_base, "Failed to free some memory (Error Code - ", error, ")");
        return true;
    }
    return false;
#endif
}

OMG_MemoryWin* omg_memory_win_create(void* omg, OMG_Kernel32* k32) {
    // TODO: function, that doesn't allocate object (omg_memory_win_init)
    HANDLE heap = k32->HeapCreate(HEAP_CREATE_ENABLE_EXECUTE, 0, 0);
    if (heap == NULL)
        return NULL;
    OMG_MemoryWin* this = k32->HeapAlloc(heap, 0, sizeof(OMG_MemoryWin));
    if (this == NULL) {
        k32->HeapDestroy(heap);
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_memory_init(this);
    base->alloc = omg_memory_win_alloc;
    base->realloc = omg_memory_win_realloc;
    base->free = omg_memory_win_free;
    base->destroy = omg_memory_win_destroy;
    OMG_END_POINTER_CAST();
    this->k32 = k32;
    this->omg = omg;
    this->heap = heap;
    base->is_allocated = true;
    return this;
}
#endif
