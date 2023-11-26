#pragma once
#include <omega/omega.h>

#if OMG_SUPPORT_WIN
#include <omega/winapi.h>

typedef struct {
    OMG_Omega parent;
    OMG_Kernel32 k32_stk;
    OMG_Kernel32* k32;
    OMG_Ntdll* nt;
    bool should_free_k32;
    bool should_free_ntdll;
} OMG_OmegaWin;

OMG_API OMG_OmegaWin* omg_win_create(OMG_EntryData* data);
OMG_API bool omg_win_init(OMG_OmegaWin* this);
#endif
