#pragma once
#include <omega/omega.h>
#include <omega/winapi.h>

#if OMG_SUPPORT_WIN
typedef struct {
    OMG_Omega parent;
    OMG_Kernel32 k32_stk;
    OMG_Kernel32* k32;
    bool should_free_k32;
} OMG_OmegaWin;

OMG_API OMG_OmegaWin* omg_win_create(OMG_EntryData* data);
OMG_API bool omg_win_init(OMG_OmegaWin* this);
#endif
