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
    if (this->k32->MultiByteToWideChar(CP_UTF8, 0, data->ptr, (int)data->len, out_buf + type_len, (int)count) <= 0) { \
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

void omg_win_fill_std(OMG_OmegaWin* this) {
    base->std->memory_allocator = base->mem;
    OMG_BEGIN_POINTER_CAST();
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
    OMG_END_POINTER_CAST();
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
    bool res = false;
    if (OMG_ISNOTNULL(file->handle)) {
        if (!file_omg->k32->CloseHandle(file->handle))
            res = true;
        file->handle = NULL;
    }
    if (OMG_ISNOTNULL(file->w_fp)) {
        OMG_FREE(file_omg_base->mem, file->w_fp);
        file->w_fp = NULL;
    }
    omg_file_destroy((OMG_File*)file);
    return res;
}

int64_t omg_win_file_get_size(OMG_FileWin* file) {
    if (OMG_ISNULL(file_omg->k32->GetFileSize)) {
        DWORD size_high = 0;
        DWORD size_low = file_omg->k32->GetFileSize(file->handle, &size_high);
        if ((size_low == INVALID_FILE_SIZE) && (size_high == 0) && (file_omg->k32->GetLastError() != NO_ERROR)) {
            _OMG_LOG_WARN(file_omg_base, "Failed to get size for Win32 file ", file_base->fp.ptr);
            return -2;
        }
        return ((int64_t)size_high << 32) | (int64_t)size_low;
    }
    LARGE_INTEGER size_buf;
    if (!file_omg->k32->GetFileSizeEx(file->handle, &size_buf)) {
        _OMG_LOG_WARN(file_omg_base, "Failed to get size for Win32 file ", file_base->fp.ptr);
        return -2;
    }
    return (int64_t)size_buf.QuadPart;
}

int64_t omg_win_file_seek(OMG_FileWin* file, int64_t offset, int whence) {
    if (OMG_ISNULL(file_omg->k32->SetFilePointerEx)) {
        _OMG_LOG_WARN(file_omg_base, "Seek in not supported for Win32 file ", file_base->fp.ptr);
        return -1;
    }
    LARGE_INTEGER res_buf;
    LARGE_INTEGER inp_val;
    inp_val.QuadPart = (LONGLONG)offset;
    if (!file_omg->k32->SetFilePointerEx(file->handle, inp_val, &res_buf, (
        (whence == OMG_FILE_SEEK_END) ? FILE_END : ((whence == OMG_FILE_SEEK_CUR) ? FILE_CURRENT : FILE_BEGIN)
    ))) {
        _OMG_LOG_WARN(file_omg_base, "Failed to seek Win32 file ", file_base->fp.ptr);
        return -2;
    }
    return (int64_t)res_buf.QuadPart;
}

size_t omg_win_file_read(OMG_FileWin* file, void* buf, size_t size, size_t maxnum) {
    if (OMG_ISNULL(file_omg->k32->ReadFile)) {
        _OMG_LOG_WARN(file_omg_base, "Reading in not supported for Win32 file ", file_base->fp.ptr);
        return 0;
    }
    DWORD bytes_read;
    if (!file_omg->k32->ReadFile(file->handle, buf, (DWORD)(size * maxnum), &bytes_read, NULL)) {
        _OMG_LOG_WARN(file_omg_base, "Failed to read Win32 file ", file_base->fp.ptr);
        return 0;
    }
    return (size_t)bytes_read;
}

size_t omg_win_file_write(OMG_FileWin* file, const void* buf, size_t size, size_t num) {
    if (OMG_ISNULL(file_omg->k32->WriteFile)) {
        _OMG_LOG_WARN(file_omg_base, "Writing in not supported for Win32 file ", file_base->fp.ptr);
        return 0;
    }
    DWORD written;
    if (!file_omg->k32->WriteFile(file->handle, buf, (DWORD)(size * num), &written, NULL)) {
        _OMG_LOG_WARN(file_omg_base, "Failed to write Win32 file ", file_base->fp.ptr);
        return 0;
    }
    return (size_t)written;
}

// TODO: Append Mode
OMG_FileWin* omg_win_file_from_fp(OMG_OmegaWin* this, OMG_FileWin* file, const OMG_String* path, int mode) {
    OMG_BEGIN_POINTER_CAST();
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    file = omg_file_from_fp(this, file, path, mode);
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
    int out_len = this->k32->MultiByteToWideChar(CP_UTF8, 0, path->ptr, (int)path->len, file->w_fp, (int)count);
    if (out_len > 0)
        file->w_fp[out_len] = L'\0';
    DWORD need_access = 0;
    if ((mode % 6) != OMG_FILE_MODE_W)
        need_access |= GENERIC_READ;
    if ((mode % 6) != OMG_FILE_MODE_R)
        need_access |= GENERIC_WRITE;
    DWORD create_mode = 0;
    if (((mode % 3) == OMG_FILE_MODE_R) || ((mode % 6) == OMG_FILE_MODE_A))
        create_mode = OPEN_EXISTING;
    else if ((mode % 3) == OMG_FILE_MODE_W)
        create_mode = CREATE_ALWAYS;
    else if ((mode % 3) == OMG_FILE_MODE_A)
        create_mode = OPEN_ALWAYS;
    DWORD shared_ops = 0;
    shared_ops |= FILE_SHARE_READ;
    file->handle = this->k32->CreateFileW(
        file->w_fp,
        need_access,
        shared_ops,
        NULL,
        create_mode,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if ((file->handle == INVALID_HANDLE_VALUE)/* || OMG_ISNULL(file->handle)*/) {
        _OMG_LOG_ERROR(base, "Failed to open Win32 file ", path->ptr);
        omg_file_destroy((OMG_File*)file);
        return NULL;
    }
    file_base->type = OMG_FILE_TYPE_WINFILE;
    file_base->destroy = omg_win_file_destroy;
    file_base->get_size = omg_win_file_get_size;
    file_base->seek = omg_win_file_seek;
    file_base->read = omg_win_file_read;
    file_base->write = omg_win_file_write;
    OMG_END_POINTER_CAST();
    return file;
}

bool omg_win_destroy_clean1(OMG_OmegaWin* this) {
#if OMG_IS_WIN
    bool result = false;
    if (this->should_free_g32 && OMG_ISNOTNULL(this->g32)) {
        result = omg_winapi_gdi32_free(this->g32) || result;
        result = OMG_FREE(base->mem, this->g32) || result;
        this->g32 = NULL;
    }
    if (this->should_free_u32 && OMG_ISNOTNULL(this->u32)) {
        result = omg_winapi_user32_free(this->u32) || result;
        result = OMG_FREE(base->mem, this->u32) || result;
        this->u32 = NULL;
    }
    if (this->should_free_uxtheme && OMG_ISNOTNULL(this->uxtheme)) {
        result = omg_winapi_uxtheme_free(this->uxtheme) || result;
        result = OMG_FREE(base->mem, this->uxtheme) || result;
        this->uxtheme = NULL;
    }
    if (this->should_free_dwm && OMG_ISNOTNULL(this->dwm)) {
        result = omg_winapi_dwmapi_free(this->dwm) || result;
        result = OMG_FREE(base->mem, this->dwm) || result;
        this->dwm = NULL;
    }
    if (this->should_free_ntdll && OMG_ISNOTNULL(this->dwm)) {
        result = omg_winapi_ntdll_free(this->nt) || result;
        result = OMG_FREE(base->mem, this->nt) || result;
        this->nt = NULL;
    }
    return result;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_win_destroy_clean2(OMG_OmegaWin* this) {
#if OMG_IS_WIN
    bool result = false;
    if (this->should_free_k32 && OMG_ISNOTNULL(this->k32)) {
        result = omg_winapi_kernel32_free(this->k32) || result;
        this->k32 = NULL;
    }
    return result;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_win_destroy(OMG_OmegaWin* this) {
    bool result = base->app_quit((OMG_Omega*)this);
    result = omg_win_destroy_clean1(this) || result;
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
    result = omg_win_destroy_clean2(this) || result;
    omg_destroy((OMG_Omega*)this);
    return result;
}

bool omg_win_loads_libs1(OMG_OmegaWin* this) {
#if OMG_IS_WIN
    if (OMG_ISNULL(this->k32)) {
        this->k32 = &this->k32_stk;
        this->should_free_k32 = false;
        if (omg_winapi_kernel32_load(this->k32))
            return true;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_omg_init(this);
    base->type = OMG_OMEGA_TYPE_WIN;
    this->con_result = 0;
    if (OMG_ISNULL(base->mem)) {
        base->mem = omg_memory_win_create(this, this->k32);
        if (OMG_ISNULL(base->mem)) {
            omg_win_destroy_clean2(this);
            return true;
        }
        base->should_free_mem = true;
    }
    if (OMG_ISNULL(this->nt)) {
        this->nt = OMG_MALLOC(base->mem, sizeof(OMG_Ntdll));
        if (OMG_ISNULL(this->nt) || omg_winapi_ntdll_load(this->nt)) {
            omg_win_destroy_clean1(this);
            if (OMG_ISNOTNULL(this->nt)) {
                OMG_FREE(base->mem, this->nt);
            }
            base->mem->destroy(base->mem);
            base->mem = NULL;
            omg_win_destroy_clean2(this);
            return true;
        }
        this->should_free_ntdll = true;
    }
    OMG_WIN_NTDLL_OSVERSIONINFOEXW os_ver_info;
    os_ver_info.dwOSVersionInfoSize = sizeof(OMG_WIN_NTDLL_OSVERSIONINFOEXW);
    this->nt->RtlGetVersion(&os_ver_info);
    this->win_major_ver = (int)os_ver_info.dwMajorVersion;
    this->win_minor_ver = (int)os_ver_info.dwMinorVersion;
    this->win_build_number = (int)os_ver_info.dwBuildNumber;
    return false;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_win_loads_libs2(OMG_OmegaWin* this) {
#if OMG_IS_WIN
    if (OMG_ISNULL(this->dwm)) {
        this->dwm = OMG_MALLOC(base->mem, sizeof(OMG_Dwmapi));
        if (OMG_ISNULL(this->dwm)) {
            return true;
        }
        omg_winapi_dwmapi_load(this->dwm);
        this->should_free_dwm = true;
    }
    if (OMG_ISNULL(this->uxtheme)) {
        this->uxtheme = OMG_MALLOC(base->mem, sizeof(OMG_Uxtheme));
        if (OMG_ISNULL(this->uxtheme)) {
            return true;
        }
        omg_winapi_uxtheme_load(this->uxtheme, this->win_build_number);
        this->should_free_uxtheme = true;
    }
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
    return false;
#else
    OMG_UNUSED(this);
    return false;
#endif
}

bool omg_win_init(OMG_OmegaWin* this) {
    this->should_free_dwm = this->should_free_g32 = this->should_free_k32 = this->should_free_ntdll = this->should_free_u32 = this->should_free_uxtheme = base->should_free_std = false;
    if (omg_win_loads_libs1(this))
        return true;
    if (OMG_ISNULL(base->std)) {
        base->std = OMG_MALLOC(base->mem, sizeof(OMG_Std));
        if (OMG_ISNULL(base->std)) {
            omg_win_destroy_clean2(this);
            return true;
        }
        omg_std_fill_defaults(base->std);
        omg_std_set_default_handle(base->std);
        omg_win_fill_std(this);
        base->should_free_std = true;
    }
    if (omg_win_loads_libs2(this)) {
        omg_win_destroy_clean1(this);
        base->mem->destroy(base->mem);
        base->mem = NULL;
        omg_win_destroy_clean2(this);
        return true;
    }
    base->sz_file = sizeof(OMG_FileWin);
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
    base->file_from_fp = omg_win_file_from_fp;
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
