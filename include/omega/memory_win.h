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
#endif
