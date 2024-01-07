#include <omega/config.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#include <omega/ostd.h>
#include <omega/memory_win.h>
#include <omega/window_win.h>
#include <omega/winmgr_win.h>
#include <omega/clock_win.h>
#include <omega/filesystem_win.h>
#define base ((OMG_Omega*)this)
#define file_base ((OMG_File*)file)
#define file_omg ((OMG_OmegaWin*)file_base->omg)
#define file_omg_base ((OMG_Omega*)file_base->omg)
#define winmgr_win ((OMG_WinmgrWin*)base->winmgr)
#define MAKE_EVENT(event) do { \
    ((OMG_Event*)event)->omg = this; \
    ((OMG_Event*)event)->data = base->event_arg; \
    ((OMG_Event*)event)->time = 0; \
} while (0)

#define _OMG_WIN_LOG_MACRO(this, data, type_str, type_len, type_len2, is_error) { \
    omg_win_attach_console(this); \
    if (this->con_result < 0) \
        return true; \
    if (is_error) { \
        if (OMG_ISNULL(this->stderr_handle)) { \
            this->stderr_handle = this->k32->GetStdHandle(STDERR_HANDLE); \
            if (OMG_ISNULL(this->stderr_handle)) { \
                return true; \
            } \
        } \
    } \
    else { \
        if (OMG_ISNULL(this->stdout_handle)) { \
            this->stdout_handle = this->k32->GetStdHandle(STDOUT_HANDLE); \
            if (OMG_ISNULL(this->stdout_handle)) { \
                return true; \
            } \
        } \
    } \
    size_t count; \
    _OMG_WIN_GET_ENCODE_SIZE(count, data, this->k32); \
    if (count == 0) \
        return true; \
    wchar_t* out_buf = OMG_MALLOC(base->mem, (size_t)count * 2 + 20); \
    if (OMG_ISNULL(out_buf)) \
        return true; \
    if (this->k32->MultiByteToWideChar(CP_UTF8, 0, data->ptr, data->len, out_buf + type_len, (int)count) <= 0) { \
        OMG_FREE(base->mem, out_buf); \
        return true; \
    } \
    base->std->memcpy(out_buf, type_str, type_len2); \
    if (out_buf[count + type_len - 1] == L'\n') { \
        count--; \
    } \
    out_buf[count + type_len] = '\n'; \
    out_buf[count + type_len + 1] = '\r'; \
    count += 2; \
    DWORD counter = 0; \
    this->k32->WriteConsoleW(is_error ? this->stderr_handle : this->stdout_handle, out_buf, (DWORD)count + 8, &counter, NULL); \
    OMG_FREE(base->mem, out_buf); \
    return false; \
}

void omg_win_fill_after_create(OMG_OmegaWin* this, OMG_EntryData* data) {
    this->k32 = NULL;
    OMG_BEGIN_POINTER_CAST();
    omg_fill_on_create(this, data);
    base->omg_init = omg_win_init;
    OMG_END_POINTER_CAST();
    this->nt = NULL;
    this->dwm = NULL;
    this->uxtheme = NULL;
}

OMG_OmegaWin* omg_win_create(OMG_EntryData* data) {
    OMG_OmegaWin* result = (OMG_OmegaWin*)omg_alloc_omega_internal(data, sizeof(OMG_OmegaWin));
    omg_win_fill_after_create(result, data);
    return result;
}

void omg_win_attach_console(OMG_OmegaWin* this) {
    if (this->con_result == 0) {
        BOOL attach_res = OMG_ISNULL(this->k32->AttachConsole) ? 0 : this->k32->AttachConsole(ATTACH_PARENT_PROCESS);
        if (!attach_res) {
            if (this->k32->GetLastError() == ERROR_ACCESS_DENIED)
                this->con_result = 1;
            else {
                if (!this->k32->AllocConsole()) {
                    this->con_result = -1;
                    return;
                }
                this->con_result = 2;
            }
        }
        else
            this->con_result = 1;
    }
}

bool omg_win_log_info_str(OMG_OmegaWin* this, const OMG_String* data) {
    _OMG_WIN_LOG_MACRO(this, data, L"[INFO]: ", 8, 16, false);
}

bool omg_win_log_warn_str(OMG_OmegaWin* this, const OMG_String* data) {
    _OMG_WIN_LOG_MACRO(this, data, L"[WARN]: ", 8, 16, false);
}

bool omg_win_log_error_str(OMG_OmegaWin* this, const OMG_String* data) {
    _OMG_WIN_LOG_MACRO(this, data, L"[ERROR]: ", 9, 18, true);
}

bool omg_win_log_fatal_str(OMG_OmegaWin* this, const OMG_String* data) {
    _OMG_WIN_LOG_MACRO(this, data, L"[FATAL]: ", 9, 18, true);
}

void omg_win_poll_events(OMG_OmegaWin* this) {
    while (base->looping && this->u32->PeekMessageW(&this->msg, NULL, 0, 0, PM_REMOVE)) {
        if (this->msg.message == WM_QUIT) {
            // Why doesn't happen in WNDPROC? Fuck Microsoft!
            // TODO: Does this work with multiple windows?
            OMG_EventQuit c_event;
            MAKE_EVENT(&c_event);
            base->on_quit(&c_event);
            continue;
        }
        if (!base->looping)
            return;
        this->u32->TranslateMessage(&this->msg);
        if (!base->looping)
            return;
        this->u32->DispatchMessageW(&this->msg);
    }
}

void omg_win_auto_loop_run(OMG_OmegaWin* this) {
    base->looping = true;
    while (base->looping) {
        omg_win_poll_events(this);
        if (!base->looping)
            break;
        OMG_EventUpdate u_event;
        MAKE_EVENT(&u_event);
        base->on_update(&u_event);
        if (!base->looping)
            break;
        for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) {
            OMG_WindowWin* win = (OMG_WindowWin*)base->winmgr->cache[i];
            if (OMG_ISNULL(win) || !((OMG_Window*)win)->enable_paint)
                continue;
            // Should I clear background???
            if (!this->u32->InvalidateRect(win->hwnd, NULL, FALSE))
                continue;
            this->u32->UpdateWindow(win->hwnd);
        }
        // this->u32->WaitMessage();
    }
    OMG_EventLoopStop ls_event;
    MAKE_EVENT(&ls_event);
    base->on_loop_stop(&ls_event);
}

void omg_win_update_scale(OMG_OmegaWin* this) {
    if (!base->support_highdpi) {
        base->scale.x = base->scale.y = 1.0f;
        return;
    }
    if (OMG_ISNOTNULL(this->u32->GetDpiForSystem))
        base->scale.x = base->scale.y = (float)this->u32->GetDpiForSystem() / 96.0f;
}

bool omg_win_app_init(OMG_OmegaWin* this) {
    base->keyboard_state = OMG_MALLOC(base->mem, OMG_NUM_SCANCODES * (sizeof(bool) + sizeof(OMG_Keycode)));
    if (OMG_ISNULL(base->keyboard_state))
        return true;
    base->keymap = (const OMG_Keycode*)((size_t)base->keyboard_state + OMG_NUM_SCANCODES * sizeof(bool));
    base->std->memset(base->keyboard_state, 0, OMG_NUM_SCANCODES * sizeof(bool));
    omg_keyboard_init_keymap(base);
    if (OMG_ISNULL(base->clock)) {
        base->clock = OMG_MALLOC(base->mem, sizeof(OMG_ClockWin));
        if (OMG_ISNULL(base->clock)) {
            OMG_FREE(base->mem, base->keyboard_state);
            base->keyboard_state = NULL;
            return true;
        }
        base->clock->was_allocated = true;
        base->clock->omg = this;
        base->clock->inited = false;
        ((OMG_ClockWin*)base->clock)->k32 = this->k32;
        OMG_BEGIN_POINTER_CAST();
        base->clock->init = omg_clock_win_init;
        OMG_END_POINTER_CAST();
    }
    if (base->support_highdpi) {
        if (OMG_ISNOTNULL(this->u32->SetProcessDpiAwarenessContext)) {
            this->u32->SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        }
        else if (OMG_ISNOTNULL(this->u32->SetProcessDpiAwareness)) {
            this->u32->SetProcessDpiAwareness(OMG_WIN_PROCESS_PER_MONITOR_DPI_AWARE);
        }
        else if (OMG_ISNOTNULL(this->u32->SetProcessDPIAware)) {
            if (!this->u32->SetProcessDPIAware()) {
                _OMG_LOG_WARN(base, "Failed to set dpi awareness for app");
            }
        }
        omg_win_update_scale(this);
    }
    if (OMG_ISNOTNULL(this->uxtheme->AllowDarkModeForApp))
        this->uxtheme->AllowDarkModeForApp((base->app_theme == OMG_THEME_DARK) || (base->app_theme == OMG_THEME_AUTO));
    if (OMG_ISNOTNULL(this->uxtheme->SetPreferredAppMode)) {
        if (base->app_theme == OMG_THEME_AUTO)
            this->uxtheme->SetPreferredAppMode(OMG_WIN_APPMODE_ALLOW_DARK);
        else if (base->app_theme == OMG_THEME_NONE)
            this->uxtheme->SetPreferredAppMode(OMG_WIN_APPMODE_DEFAULT);
        else if (base->app_theme == OMG_THEME_LIGHT)
            this->uxtheme->SetPreferredAppMode(OMG_WIN_APPMODE_FORCE_LIGHT);
        else if (base->app_theme == OMG_THEME_DARK)
            this->uxtheme->SetPreferredAppMode(OMG_WIN_APPMODE_FORCE_DARK);
    }
    if (OMG_ISNOTNULL(this->uxtheme->ShouldSystemUseDarkMode))
        base->theme = this->uxtheme->ShouldSystemUseDarkMode() ? OMG_THEME_DARK : OMG_THEME_LIGHT;
    base->inited = true;
    return false;
}

void omg_win_delay(OMG_OmegaWin* this, double seconds) {
    this->k32->Sleep((DWORD)(seconds * 1000.0));
}

bool omg_win_app_quit(OMG_OmegaWin* this) {
    if (base->inited) {
        omg_app_quit((OMG_Omega*)this);
        base->inited = false;
    }
    return false;
}

bool omg_win_destroy(OMG_OmegaWin* this) {
    bool result = base->app_quit((OMG_Omega*)this);
    // TODO: Null Checks
    if (this->should_free_g32) {
        result = omg_winapi_gdi32_free(this->g32) || result;
        result = OMG_FREE(base->mem, this->g32) || result;
        this->g32 = NULL;
    }
    if (this->should_free_u32) {
        result = omg_winapi_user32_free(this->u32) || result;
        result = OMG_FREE(base->mem, this->u32) || result;
        this->u32 = NULL;
    }
    if (this->should_free_uxtheme) {
        result = omg_winapi_uxtheme_free(this->uxtheme) || result;
        result = OMG_FREE(base->mem, this->uxtheme) || result;
        this->uxtheme = NULL;
    }
    if (this->should_free_dwm) {
        result = omg_winapi_dwmapi_free(this->dwm) || result;
        result = OMG_FREE(base->mem, this->dwm) || result;
        this->dwm = NULL;
    }
    if (this->should_free_ntdll) {
        result = omg_winapi_ntdll_free(this->nt) || result;
        result = OMG_FREE(base->mem, this->nt) || result;
        this->nt = NULL;
    }
    if (this->con_result > 1) {
        result = !this->k32->FreeConsole() || result;
        this->con_result = 0;
    }
    if (base->should_free_std) {
        result = OMG_FREE(base->mem, base->std) || result;
        base->std = NULL;
    }
    if (base->should_free_mem) {
        result = base->mem->destroy(base->mem) || result;
        base->mem = NULL;
    }
    if (this->should_free_k32) {
        result = omg_winapi_kernel32_free(this->k32) || result;
        this->k32 = NULL;
    }
    omg_destroy((OMG_Omega*)this);
    return result;
}

void omg_win_fill_std(OMG_OmegaWin* this) {
    base->std->memory_allocator = base->mem;
    // TODO
    /* base->std->lib_load = omg_win_std_lib_load;
    base->std->lib_func = omg_win_std_lib_func;
    base->std->lib_free = omg_win_std_lib_free; */
    base->std->floor = this->nt->floor;
    base->std->ceil = this->nt->ceil;
    base->std->sin = this->nt->sin;
    base->std->cos = this->nt->cos;
    base->std->tan = this->nt->tan;
    base->std->atan = this->nt->atan;
    base->std->atan2 = this->nt->atan2;
    base->std->sqrt = this->nt->sqrt;
    base->std->qsort = this->nt->qsort;
    base->std->abs = this->nt->abs;
    base->std->fabs = this->nt->fabs;
    base->std->pow = this->nt->pow;
}

bool omg_win_alloc_winmgr(OMG_OmegaWin* this) {
    base->winmgr = OMG_MALLOC(base->mem, sizeof(OMG_WinmgrWin));
    if (OMG_ISNULL(base->winmgr))
        return true;
    omg_alloc_winmgr((OMG_Omega*)this);
    winmgr_win->u32 = this->u32;
    winmgr_win->g32 = this->g32;
    winmgr_win->k32 = this->k32;
    winmgr_win->dwm = this->dwm;
    winmgr_win->uxtheme = this->uxtheme;
    winmgr_win->win_build_num = this->win_build_number;
    OMG_BEGIN_POINTER_CAST();
    base->winmgr->init = omg_winmgr_win_init;
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_win_file_destroy(OMG_FileWin* file) {
    if (OMG_ISNOTNULL(file->w_fp)) {
        OMG_FREE(file_omg_base->mem, file->w_fp);
        file->w_fp = NULL;
    }
    omg_file_destroy((OMG_File*)file);
    return false;
}

OMG_FileWin* omg_win_file_from_path(OMG_OmegaWin* this, OMG_FileWin* file, const OMG_String* path, int mode) {
    OMG_BEGIN_POINTER_CAST();
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    file = omg_file_from_path(this, file, path, mode);
    if (OMG_ISNULL(file))
        return NULL;
    size_t count;
    _OMG_WIN_GET_ENCODE_SIZE(count, path, this->k32);
    if (count == 0) {
        _OMG_LOG_ERROR(base, "Failed to open Win32 file ", path->ptr);
        omg_file_destroy((OMG_File*)file);
        return NULL;
    }
    file->w_fp = OMG_MALLOC(base->mem, (size_t)count * 2 + 2);
    if (OMG_ISNULL(file->w_fp)) {
        _OMG_LOG_ERROR(base, "Failed to open Win32 file ", path->ptr);
        omg_file_destroy((OMG_File*)file);
        return NULL;
    }
    int out_len = this->k32->MultiByteToWideChar(CP_UTF8, 0, path->ptr, path->len, file->w_fp, (int)count);
    if (out_len > 0)
        file->w_fp[out_len] = L'\0';
    file_base->destroy = omg_win_file_destroy;
    OMG_END_POINTER_CAST();
    return file;
}

bool omg_win_init(OMG_OmegaWin* this) {
    // TODO: cleanups on errors
    if (OMG_ISNULL(this->k32)) {
        this->k32 = &this->k32_stk;
        if (omg_winapi_kernel32_load(this->k32))
            return true;
        this->should_free_k32 = true;
    }
    else
        this->should_free_k32 = false;
    OMG_BEGIN_POINTER_CAST();
    omg_omg_init(this);
    base->type = OMG_OMEGA_TYPE_WIN;
    this->con_result = 0;
    if (OMG_ISNULL(base->mem)) {
        base->mem = omg_memory_win_create(this, this->k32);
        if (OMG_ISNULL(base->mem)) {
            if (this->should_free_k32) {
                omg_winapi_kernel32_free(this->k32);
                this->k32 = NULL;
            }
            return true;
        }
        base->should_free_mem = true;
    }
    else
        base->should_free_mem = false;
    if (OMG_ISNULL(this->nt)) {
        this->nt = OMG_MALLOC(base->mem, sizeof(OMG_Ntdll));
        if (OMG_ISNULL(this->nt)) {
            base->mem->destroy(base->mem);
            base->mem = NULL;
            omg_winapi_kernel32_free(this->k32);
            this->k32 = NULL;
            return true;
        }
        if (omg_winapi_ntdll_load(this->nt)) {
            OMG_FREE(base->mem, this->nt);
            base->mem->destroy(base->mem);
            base->mem = NULL;
            omg_winapi_kernel32_free(this->k32);
            this->k32 = NULL;
            return true;
        }
        this->should_free_ntdll = true;
    }
    else
        this->should_free_ntdll = false;
    if (OMG_ISNULL(base->std)) {
        base->std = OMG_MALLOC(base->mem, sizeof(OMG_Std));
        if (OMG_ISNULL(base->std)) {
            if (this->should_free_k32) {
                omg_winapi_kernel32_free(this->k32);
                this->k32 = NULL;
            }
            return true;
        }
        omg_std_fill_defaults(base->std);
        omg_std_set_default_handle(base->std);
        omg_win_fill_std(this);
        base->should_free_std = true;
    }
    else
        base->should_free_std = false;
    OMG_WIN_NTDLL_OSVERSIONINFOEXW os_ver_info;
    os_ver_info.dwOSVersionInfoSize = sizeof(OMG_WIN_NTDLL_OSVERSIONINFOEXW);
    this->nt->RtlGetVersion(&os_ver_info);
    this->win_major_ver = (int)os_ver_info.dwMajorVersion;
    this->win_minor_ver = (int)os_ver_info.dwMinorVersion;
    this->win_build_number = (int)os_ver_info.dwBuildNumber;
    base->sz_file = sizeof(OMG_FileWin);
    if (OMG_ISNULL(this->dwm)) {
        this->dwm = OMG_MALLOC(base->mem, sizeof(OMG_Dwmapi));
        if (OMG_ISNULL(this->dwm)) {
            return true;
        }
        omg_winapi_dwmapi_load(this->dwm);
        this->should_free_dwm = true;
    }
    else
        this->should_free_dwm = false;
    if (OMG_ISNULL(this->uxtheme)) {
        this->uxtheme = OMG_MALLOC(base->mem, sizeof(OMG_Uxtheme));
        if (OMG_ISNULL(this->uxtheme)) {
            return true;
        }
        omg_winapi_uxtheme_load(this->uxtheme, this->win_build_number);
        this->should_free_uxtheme = true;
    }
    else
        this->should_free_uxtheme = false;
    if (OMG_ISNULL(this->u32)) {
        this->u32 = OMG_MALLOC(base->mem, sizeof(OMG_User32));
        if (OMG_ISNULL(this->u32)) {
            return true;
        }
        if (omg_winapi_user32_load(this->u32)) {
            OMG_FREE(base->mem, this->u32);
            return true;
        }
        this->should_free_u32 = true;
    }
    else
        this->should_free_u32 = false;
    if (OMG_ISNULL(this->g32)) {
        this->g32 = OMG_MALLOC(base->mem, sizeof(OMG_Gdi32));
        if (OMG_ISNULL(this->g32)) {
            return true;
        }
        if (omg_winapi_gdi32_load(this->g32)) {
            OMG_FREE(base->mem, this->g32);
            return true;
        }
        this->should_free_g32 = true;
    }
    else
        this->should_free_g32 = false;
    base->app_init = omg_win_app_init;
    base->app_quit = omg_win_app_quit;
    base->delay = omg_win_delay;
    base->log_info_str = omg_win_log_info_str;
    base->log_warn_str = omg_win_log_warn_str;
    base->log_error_str = omg_win_log_error_str;
    base->log_fatal_str = omg_win_log_fatal_str;
    base->auto_loop_run = omg_win_auto_loop_run;
    base->winmgr_alloc = omg_win_alloc_winmgr;
    base->destroy = omg_win_destroy;
    base->file_from_path = omg_win_file_from_path;
    OMG_END_POINTER_CAST();
    _OMG_LOG_INFO(base, "Omega successfully inited with Win32 backend");
    // TODO: correct version by check build number
    _OMG_LOG_INFO(
        base, "Running on Windows ",
        (this->win_major_ver == 10 && this->win_build_number >= 21996) ? 11 : this->win_major_ver, // Fuck Microsoft
        " build ", this->win_build_number
    );
    return false;
}
#endif
