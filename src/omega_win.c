#include <omega/config.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#include <omega/ostd.h>
#include <omega/memory_win.h>
#include <omega/window_win.h>
#define base ((OMG_Omega*)this)

#define _OMG_WIN_LOG_MACRO(this, data, type_str, type_len, type_len2, is_error) { \
    omg_win_attach_console(this); \
    if (this->con_result < 0) \
        return true; \
    if (is_error) { \
        if (OMG_ISNULL(this->stderr_handle)) { \
            this->stderr_handle = this->k32->GetStdHandle(OMG_WIN_STD_ERROR_HANDLE); \
            if (OMG_ISNULL(this->stderr_handle)) { \
                return true; \
            } \
        } \
    } \
    else { \
        if (OMG_ISNULL(this->stdout_handle)) { \
            this->stdout_handle = this->k32->GetStdHandle(OMG_WIN_STD_OUTPUT_HANDLE); \
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
    if (this->k32->MultiByteToWideChar(OMG_WIN_CP_UTF8, 0, data->ptr, data->len, out_buf + type_len, (int)count) <= 0) { \
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
    static OMG_OmegaWin result;
    omg_win_fill_after_create(&result, data);
    return &result;
}

void omg_win_attach_console(OMG_OmegaWin* this) {
    if (this->con_result == 0) {
        BOOL attach_res = this->k32->AttachConsole(OMG_WIN_ATTACH_PARENT_PROCESS);
        if (!attach_res) {
            if (this->k32->GetLastError() == OMG_WIN_ERROR_ACCESS_DENIED)
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

bool omg_win_app_init(OMG_OmegaWin* this) {
    if (base->support_highdpi) {
        if (OMG_ISNOTNULL(this->u32->SetProcessDPIAware)) {
            if (!this->u32->SetProcessDPIAware()) {
                _OMG_LOG_WARN(base, "Failed to set dpi awareness for app");
            }
        }
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
    return false;
}

void omg_win_delay(OMG_OmegaWin* this, float seconds) {
    this->k32->Sleep((DWORD)(seconds * 1000.0f));
}

bool omg_win_app_quit(OMG_OmegaWin* this) {
    // Auto-free everything allocated like windows
    OMG_UNUSED(this);
    return false;
}

bool omg_win_destroy(OMG_OmegaWin* this) {
    bool result = false;
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
}

OMG_WindowWin* omg_win_window_alloc(OMG_OmegaWin* this) {
    OMG_WindowWin* result = OMG_MALLOC(base->mem, sizeof(OMG_WindowWin));
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(base, "Failed to allocate memory for Win32 Window");
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_window_fill_on_create(result);
    result->parent.omg = base;
    result->omg = this;
    result->u32 = this->u32;
    result->k32 = this->k32;
    result->dwm = this->dwm;
    result->uxtheme = this->uxtheme;
    result->parent.default_init = omg_window_win_init;
    OMG_END_POINTER_CAST();
    return result;
}

bool omg_win_init(OMG_OmegaWin* this) {
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
    OMG_WIN_NTDLL_OSVERSIONINFOEXW os_ver_info;
    os_ver_info.dwOSVersionInfoSize = sizeof(OMG_WIN_NTDLL_OSVERSIONINFOEXW);
    this->nt->RtlGetVersion(&os_ver_info);
    this->win_major_ver = (int)os_ver_info.dwMajorVersion;
    this->win_minor_ver = (int)os_ver_info.dwMinorVersion;
    this->win_build_number = (int)os_ver_info.dwBuildNumber;
    // TODO: cleanups on errors
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
    base->app_init = omg_win_app_init;
    base->app_quit = omg_win_app_quit;
    base->delay = omg_win_delay;
    base->log_info_str = omg_win_log_info_str;
    base->log_warn_str = omg_win_log_warn_str;
    base->log_error_str = omg_win_log_error_str;
    base->log_fatal_str = omg_win_log_fatal_str;
    base->window_alloc = omg_win_window_alloc;
    base->destroy = omg_win_destroy;
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
