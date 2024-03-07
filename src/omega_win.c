#include <omega/config.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#include <omega/ostd.h>
#include <omega/memory_win.h>
#include <omega/window_win.h>
#include <omega/winmgr_win.h>
#include <omega/clock_win.h>
#include <omega/filesystem_win.h>
#define d_k32 ((OMG_Kernel32*)base->k32)
#define d_u32 ((OMG_User32*)base->u32)
#define d_nt ((OMG_Ntdll*)base->nt)
#define d_uxtheme ((OMG_Uxtheme*)base->uxtheme)
#define base ((OMG_Omega*)this)
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
            this->stderr_handle = d_k32->GetStdHandle(STDERR_HANDLE); \
            if (OMG_ISNULL(this->stderr_handle)) { \
                return true; \
            } \
        } \
    } \
    else { \
        if (OMG_ISNULL(this->stdout_handle)) { \
            this->stdout_handle = d_k32->GetStdHandle(STDOUT_HANDLE); \
            if (OMG_ISNULL(this->stdout_handle)) { \
                return true; \
            } \
        } \
    } \
    size_t count; \
    _OMG_WIN_GET_ENCODE_SIZE(count, data, d_k32); \
    if (count == 0) \
        return true; \
    wchar_t* out_buf = OMG_MALLOC(base->mem, (size_t)count * 2 + 20); \
    if (OMG_ISNULL(out_buf)) \
        return true; \
    if (d_k32->MultiByteToWideChar(CP_UTF8, 0, data->ptr, (int)data->len, out_buf + type_len, (int)count) <= 0) { \
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
    d_k32->WriteConsoleW(is_error ? this->stderr_handle : this->stdout_handle, out_buf, (DWORD)count + 8, &counter, NULL); \
    OMG_FREE(base->mem, out_buf); \
    return false; \
}

void omg_win_fill_after_create(OMG_OmegaWin* this, OMG_EntryData* data) {
    OMG_BEGIN_POINTER_CAST();
    omg_fill_on_create(this, data);
    base->omg_init = omg_win_init;
    OMG_END_POINTER_CAST();
    base->nt = NULL;
    base->dwm = NULL;
    base->uxtheme = NULL;
}

OMG_OmegaWin* omg_win_create(OMG_EntryData* data) {
    OMG_OmegaWin* result = (OMG_OmegaWin*)omg_alloc_omega_internal(data, sizeof(OMG_OmegaWin));
    omg_win_fill_after_create(result, data);
    return result;
}

void omg_win_attach_console(OMG_OmegaWin* this) {
    if (this->con_result == 0) {
        BOOL attach_res = OMG_ISNULL(d_k32->AttachConsole) ? 0 : d_k32->AttachConsole(ATTACH_PARENT_PROCESS);
        if (!attach_res) {
            if (d_k32->GetLastError() == ERROR_ACCESS_DENIED)
                this->con_result = 1;
            else {
                if (!d_k32->AllocConsole()) {
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

bool omg_win_log_str_type(OMG_OmegaWin* this, const OMG_String* data, int type) {
    wchar_t* header_text = L"[FATAL]: ";
    size_t type_len1 = 9;
    size_t type_len2 = 18;
    bool is_stderr = true;
    if (type == OMG_LOG_CATEGORY_INFO) {
        is_stderr = false;
        type_len1 = 8;
        type_len2 = 16;
        header_text = L"[INFO]: ";
    }
    else if (type == OMG_LOG_CATEGORY_WARN) {
        is_stderr = false;
        type_len1 = 8;
        type_len2 = 16;
        header_text = L"[WARN]: ";
    }
    else if (type == OMG_LOG_CATEGORY_ERROR)
        header_text = L"[ERROR]: ";
    _OMG_WIN_LOG_MACRO(this, data, header_text, type_len1, type_len2, is_stderr);
}

void omg_win_poll_events(OMG_OmegaWin* this) {
    while (base->looping && d_u32->PeekMessageW(&this->msg, NULL, 0, 0, PM_REMOVE)) {
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
        d_u32->TranslateMessage(&this->msg);
        if (!base->looping)
            return;
        d_u32->DispatchMessageW(&this->msg);
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
            if (!d_u32->InvalidateRect(win->hwnd, NULL, FALSE))
                continue;
            d_u32->UpdateWindow(win->hwnd);
        }
        // d_u32->WaitMessage();
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
    if (OMG_ISNOTNULL(d_u32->GetDpiForSystem))
        base->scale.x = base->scale.y = (float)d_u32->GetDpiForSystem() / 96.0f;
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
        ((OMG_ClockWin*)base->clock)->k32 = d_k32;
        OMG_BEGIN_POINTER_CAST();
        base->clock->init = omg_clock_win_init;
        OMG_END_POINTER_CAST();
    }
    if (base->support_highdpi) {
        if (OMG_ISNOTNULL(d_u32->SetProcessDpiAwarenessContext)) {
            d_u32->SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        }
        else if (OMG_ISNOTNULL(d_u32->SetProcessDpiAwareness)) {
            d_u32->SetProcessDpiAwareness(OMG_WIN_PROCESS_PER_MONITOR_DPI_AWARE);
        }
        else if (OMG_ISNOTNULL(d_u32->SetProcessDPIAware)) {
            if (!d_u32->SetProcessDPIAware()) {
                _OMG_LOG_WARN(base, "Failed to set dpi awareness for app");
            }
        }
        omg_win_update_scale(this);
    }
    _OMG_CHECK_DARK_MODE_INTERNAL();
    base->inited = true;
    return false;
}

void omg_win_delay(OMG_OmegaWin* this, double seconds) {
    d_k32->Sleep((DWORD)(seconds * 1000.0));
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
    base->std->floor = d_nt->floor;
    base->std->ceil = d_nt->ceil;
    base->std->sin = d_nt->sin;
    base->std->cos = d_nt->cos;
    base->std->tan = d_nt->tan;
    base->std->atan = d_nt->atan;
    base->std->atan2 = d_nt->atan2;
    base->std->sqrt = d_nt->sqrt;
    base->std->qsort = d_nt->qsort;
    base->std->abs = d_nt->abs;
    base->std->fabs = d_nt->fabs;
    base->std->pow = d_nt->pow;
    base->std->sscanf = d_nt->sscanf_s;
    OMG_END_POINTER_CAST();
}

bool omg_win_alloc_winmgr(OMG_OmegaWin* this) {
    base->winmgr = OMG_MALLOC(base->mem, sizeof(OMG_WinmgrWin));
    if (OMG_ISNULL(base->winmgr))
        return true;
    omg_alloc_winmgr((OMG_Omega*)this);
    winmgr_win->u32 = d_u32;
    winmgr_win->g32 = base->g32;
    winmgr_win->k32 = d_k32;
    winmgr_win->dwm = base->dwm;
    winmgr_win->uxtheme = d_uxtheme;
    winmgr_win->win_build_num = base->win_build_number;
    OMG_BEGIN_POINTER_CAST();
    base->winmgr->init = omg_winmgr_win_init;
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_win_destroy(OMG_OmegaWin* this) {
    bool result = base->app_quit((OMG_Omega*)this);
    result = omg_win_destroy_clean1(base) || result;
    if (this->con_result > 1) {
        result = !d_k32->FreeConsole() || result;
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
    result = omg_win_destroy_clean2(base) || result;
    omg_destroy((OMG_Omega*)this);
    return result;
}

bool omg_win_init(OMG_OmegaWin* this) {
    omg_omg_init(base);
    this->con_result = 0;
    base->type = OMG_OMEGA_TYPE_WIN;
    if (omg_win_loads_libs1(base))
        return true;
    if (OMG_ISNULL(base->mem)) {
        base->mem = (OMG_Memory*)omg_memory_win_create(this, base->k32);
        if (OMG_ISNULL(base->mem)) {
            omg_win_destroy_clean2(base);
            return true;
        }
        base->should_free_mem = true;
    }
    if (omg_win_loads_libs2(base)) {
        omg_win_destroy_clean1(base);
        base->mem->destroy(base->mem);
        base->mem = NULL;
        omg_win_destroy_clean2(base);
        return true;
    }
    if (OMG_ISNULL(base->std)) {
        base->std = OMG_MALLOC(base->mem, sizeof(OMG_Std));
        if (OMG_ISNULL(base->std)) {
            omg_win_destroy_clean2(base);
            return true;
        }
        omg_std_fill_defaults(base->std);
        omg_std_set_default_handle(base->std);
        omg_win_fill_std(this);
        base->should_free_std = true;
    }
    if (omg_win_loads_libs3(base)) {
        omg_win_destroy_clean1(base);
        base->mem->destroy(base->mem);
        base->mem = NULL;
        omg_win_destroy_clean2(base);
        return true;
    }
    OMG_BEGIN_POINTER_CAST();
    base->app_init = omg_win_app_init;
    base->app_quit = omg_win_app_quit;
    base->delay = omg_win_delay;
    base->log_str_type = omg_win_log_str_type;
    base->auto_loop_run = omg_win_auto_loop_run;
    base->winmgr_alloc = omg_win_alloc_winmgr;
    base->destroy = omg_win_destroy;
    OMG_END_POINTER_CAST();
    _OMG_LOG_INFO(base, "Omega successfully inited with Win32 backend");
    // TODO: correct version by check build number
    /* _OMG_LOG_INFO(
        base, "Running on Windows ",
        (base->win_major_ver == 10 && base->win_build_number >= 21996) ? 11 : base->win_major_ver, // Fuck Microsoft
        " build ", base->win_build_number
    ); */
    return false;
}
#endif
