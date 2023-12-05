#pragma once
#include <omega/memory.h>

#if OMG_SUPPORT_WIN
#include <omega/winapi.h>

typedef struct {
    OMG_Memory parent;
    HANDLE heap;
    OMG_Kernel32* k32;
    void* omg;
} OMG_MemoryWin;

OMG_API OMG_MemoryWin* omg_memory_win_create(void* omg, OMG_Kernel32* k32);
#if OMG_EXPORT_SHIT
OMG_API bool omg_memory_win_destroy(OMG_MemoryWin* this);
OMG_API void* omg_memory_win_alloc(OMG_MemoryWin* this, OMG_MemoryExtra extra);
OMG_API void* omg_memory_win_realloc(OMG_MemoryWin* this, void* ptr, size_t size);
OMG_API bool omg_memory_win_free(OMG_MemoryWin* this, void* ptr);
#endif
#endif
