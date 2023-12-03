#pragma once
#include <omega/omega.h>

#if OMG_SUPPORT_WIN
#include <omega/winapi.h>

typedef struct {
    OMG_Omega parent;
    OMG_Kernel32 k32_stk;
    OMG_Kernel32* k32;
    OMG_Ntdll* nt;
    HANDLE stdout_handle;
    HANDLE stderr_handle;
    int con_result;
    int win_major_ver;
    int win_minor_ver;
    int win_build_number;
    bool should_free_k32;
    bool should_free_ntdll;
} OMG_OmegaWin;

OMG_API OMG_OmegaWin* omg_win_create(OMG_EntryData* data);
OMG_API bool omg_win_init(OMG_OmegaWin* this);
OMG_API void* omg_win_std_lib_load(const OMG_String* fn);
OMG_API void* omg_win_std_lib_func(void* lib, const OMG_String* func_name);
OMG_API bool omg_win_std_lib_free(void* lib);
OMG_API void omg_win_fill_std(OMG_OmegaWin* this);
#endif
