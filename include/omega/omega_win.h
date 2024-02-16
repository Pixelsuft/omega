#pragma once
#include <omega/omega.h>

#if OMG_SUPPORT_WIN
#include <omega/api_win.h>

typedef struct {
    OMG_Omega parent;
    // OMG_Kernel32 k32_stk;
    MSG msg;
    /* OMG_Kernel32* k32;
    OMG_Ntdll* nt;
    OMG_Dwmapi* dwm;
    OMG_User32* u32;
    OMG_Gdi32* g32;
    OMG_Uxtheme* uxtheme; */
    HANDLE stdout_handle;
    HANDLE stderr_handle;
    int con_result;
    /* int win_major_ver;
    int win_minor_ver;
    int win_build_number;
    bool should_free_k32;
    bool should_free_ntdll;
    bool should_free_dwm;
    bool should_free_u32;
    bool should_free_g32;
    bool should_free_uxtheme; */
} OMG_OmegaWin;

OMG_API OMG_OmegaWin* omg_win_create(OMG_EntryData* data);
OMG_API bool omg_win_init(OMG_OmegaWin* this);
OMG_API void omg_win_fill_std(OMG_OmegaWin* this);
OMG_API void omg_win_fill_after_create(OMG_OmegaWin* this, OMG_EntryData* data);
OMG_API bool omg_win_destroy(OMG_OmegaWin* this);
OMG_API void omg_win_poll_events(OMG_OmegaWin* this);
OMG_API void omg_win_update_scale(OMG_OmegaWin* this);
#if OMG_EXPORT_SHIT
#include <omega/window_win.h>
#include <omega/filesystem_win.h>

OMG_API OMG_FileWin* omg_win_file_from_fp(OMG_OmegaWin* this, OMG_FileWin* file, const OMG_String* path, int mode);
OMG_API bool omg_win_file_destroy(OMG_FileWin* file);
OMG_API int64_t omg_win_file_get_size(OMG_FileWin* file);
OMG_API int64_t omg_win_file_seek(OMG_FileWin* file, int64_t offset, int whence);
OMG_API size_t omg_win_file_read(OMG_FileWin* file, void* buf, size_t size, size_t maxnum);
OMG_API size_t omg_win_file_write(OMG_FileWin* file, const void* buf, size_t size, size_t num);
OMG_API void omg_win_attach_console(OMG_OmegaWin* this);
OMG_API bool omg_win_app_init(OMG_OmegaWin* this);
OMG_API bool omg_win_app_quit(OMG_OmegaWin* this);
OMG_API bool omg_win_alloc_winmgr(OMG_OmegaWin* this);
OMG_API void omg_win_auto_loop_run(OMG_OmegaWin* this);
OMG_API void omg_win_delay(OMG_OmegaWin* this, double seconds);
OMG_API LRESULT omg_win_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
OMG_API bool omg_win_log_str_type(OMG_OmegaWin* this, const OMG_String* data, int type);
#endif
#endif
