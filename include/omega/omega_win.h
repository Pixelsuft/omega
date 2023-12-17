#pragma once
#include <omega/omega.h>

#if OMG_SUPPORT_WIN
#include <omega/api_win.h>

typedef struct {
    OMG_Omega parent;
    OMG_Kernel32 k32_stk;
    OMG_Kernel32* k32;
    OMG_Ntdll* nt;
    OMG_Dwmapi* dwm;
    OMG_User32* u32;
    OMG_Uxtheme* uxtheme;
    HANDLE stdout_handle;
    HANDLE stderr_handle;
    int con_result;
    int win_major_ver;
    int win_minor_ver;
    int win_build_number;
    bool should_free_k32;
    bool should_free_ntdll;
    bool should_free_dwm;
    bool should_free_u32;
    bool should_free_uxtheme;
} OMG_OmegaWin;

OMG_API OMG_OmegaWin* omg_win_create(OMG_EntryData* data);
OMG_API bool omg_win_init(OMG_OmegaWin* this);
OMG_API void omg_win_fill_std(OMG_OmegaWin* this);
OMG_API void omg_win_fill_after_create(OMG_OmegaWin* this, OMG_EntryData* data);
OMG_API bool omg_win_destroy(OMG_OmegaWin* this);
#if OMG_EXPORT_SHIT
#include <omega/window_win.h>
OMG_API void omg_win_attach_console(OMG_OmegaWin* this);
OMG_API bool omg_win_app_init(OMG_OmegaWin* this);
OMG_API bool omg_win_app_quit(OMG_OmegaWin* this);
OMG_API void omg_win_delay(OMG_OmegaWin* this, float seconds);
OMG_API bool omg_win_log_info_str(OMG_OmegaWin* this, const OMG_String* data);
OMG_API bool omg_win_log_warn_str(OMG_OmegaWin* this, const OMG_String* data);
OMG_API bool omg_win_log_error_str(OMG_OmegaWin* this, const OMG_String* data);
OMG_API bool omg_win_log_fatal_str(OMG_OmegaWin* this, const OMG_String* data);
OMG_API OMG_WindowWin* omg_win_window_alloc(OMG_OmegaWin* this);
#endif
#endif
