#pragma once
#include <omega/memory.h>
#include <omega/winapi.h>

#if OMG_SUPPORT_WIN
typedef struct {
    OMG_Memory parent;
    HANDLE heap;
    OMG_Kernel32* k32;
} OMG_MemoryWin;

OMG_API OMG_MemoryWin* omg_memory_win_create(OMG_Kernel32* data);
#endif
