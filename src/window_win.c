#include <omega/window_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega.h>
#include <omega/renderer_sdl2.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define ren_sdl2 ((OMG_RendererSdl2*)base->ren)
#define RET_DEF_PROC() this->u32->DefWindowProcW(hwnd, msg, wparam, lparam)
#ifdef SetWindowLongPtrW
#define GET_WIN_STYLE() (int64_t)this->u32->GetWindowLongW(this->hwnd, GWL_STYLE)
#define SET_WIN_STYLE(value) this->u32->SetWindowLongW(this->hwnd, GWL_STYLE, (LONG)(value))
#else
#define GET_WIN_STYLE() (int64_t)this->u32->GetWindowLongPtrW(this->hwnd, GWL_STYLE)
#define SET_WIN_STYLE(value) this->u32->SetWindowLongPtrW(this->hwnd, GWL_STYLE, (LONG_PTR)(value))
#endif
#define MAKE_EVENT(event) do { \
    ((OMG_Event*)event)->omg = base->omg; \
    ((OMG_Event*)event)->data = omg_base->event_arg; \
    ((OMG_Event*)event)->time = 0; \
} while (0)

bool omg_window_win_show(OMG_WindowWin* this, bool show) {
    this->u32->ShowWindow(this->hwnd, show ? SW_SHOWNORMAL : SW_HIDE);
    return false;
}

static LONG OMG_WIN_STD_PREFIX (*OMG_WIN_CB_GetWindowLongW)(HWND, int) = NULL;
static LONG_PTR OMG_WIN_STD_PREFIX (*OMG_WIN_CB_GetWindowLongPtrW)(HWND, int) = NULL;

void omg_window_win_check_dark_mode(OMG_WindowWin* this) {
    if (OMG_ISNOTNULL(this->uxtheme->ShouldAppsUseDarkMode) && (true || (omg_base->theme == OMG_THEME_NONE))) {
        omg_base->theme = this->uxtheme->ShouldAppsUseDarkMode() ? OMG_THEME_DARK : OMG_THEME_LIGHT;
    }
    if (OMG_ISNOTNULL(this->uxtheme->AllowDarkModeForWindow)) {
        this->uxtheme->AllowDarkModeForWindow(
            this->hwnd,
            ((omg_base->app_theme == OMG_THEME_DARK) || (omg_base->app_theme == OMG_THEME_AUTO)) && (omg_base->theme != OMG_THEME_LIGHT)
        );
    }
    if (OMG_ISNOTNULL(this->dwm->DwmSetWindowAttribute)) {
        DWORD val = (omg_base->theme == OMG_THEME_DARK) ? 1 : 0;
        this->dwm->DwmSetWindowAttribute(this->hwnd, 19, &val, sizeof(DWORD));
        val = (omg_base->theme == OMG_THEME_DARK) ? 1 : 0;
        this->dwm->DwmSetWindowAttribute(this->hwnd, 20, &val, sizeof(DWORD));
    }
}

bool omg_window_win_set_title(OMG_WindowWin* this, const OMG_String* new_title) {
    size_t count;
    _OMG_WIN_GET_ENCODE_SIZE(count, new_title, this->k32);
    if (count == 0)
        return true;
    wchar_t* out_buf = OMG_MALLOC(omg_base->mem, (size_t)count * 2 + 2);
    if (OMG_ISNULL(out_buf))
        return true;
    bool result = true;
    int out_len = this->k32->MultiByteToWideChar(CP_UTF8, 0, new_title->ptr, new_title->len, out_buf, (int)count);
    if (out_len > 0) {
        out_buf[out_len] = L'\0';
        result = !this->u32->SetWindowTextW(this->hwnd, out_buf);
    }
    OMG_FREE(omg_base->mem, out_buf);
    return result;
}

bool omg_window_win_set_state(OMG_WindowWin* this, int state) {
    if (state & OMG_WIN_STATE_MAXIMIZED)
        this->u32->ShowWindow(this->hwnd, SW_SHOWMAXIMIZED);
    if (state & OMG_WIN_STATE_MINIMIZED)
        this->u32->ShowWindow(this->hwnd, SW_MINIMIZE);
    if (state & OMG_WIN_STATE_RESTORED)
        this->u32->ShowWindow(this->hwnd, SW_RESTORE);
    this->u32->SetWindowPos(
        this->hwnd, NULL, 0, 0, 0, 0,
        SWP_NOZORDER | ((state & OMG_WIN_STATE_RESTORED) ? 0 : SWP_NOACTIVATE) | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED
    );
    return false;
}

bool omg_window_win_set_sys_button(OMG_WindowWin* this, int id, bool enabled) {
    int64_t style = GET_WIN_STYLE();
    if (!style)
        return true;
    int64_t index = (id & OMG_WIN_SYS_BUTTON_CLOSE) ? WS_SYSMENU : (
        (id & OMG_WIN_SYS_BUTTON_MAXIMIZE) ? WS_MAXIMIZEBOX : (
            (id & OMG_WIN_SYS_BUTTON_MINIMIZE) ? WS_MINIMIZEBOX : 0
        )
    );
    if (enabled) {
        base->sys_buttons |= id;
        style |= index;
    }
    else {
        base->sys_buttons &= ~id;
        style &= ~index;
    }
    // TODO: fuck microsoft because of close button
    return !SET_WIN_STYLE(style);
}

bool omg_window_win_set_resizable(OMG_WindowWin* this, bool enabled) {
    int64_t style = GET_WIN_STYLE();
    if (!style)
        return true;
    base->resizable = enabled;
    if (enabled)
        style |= WS_THICKFRAME;
    else
        style &= ~WS_THICKFRAME;
    bool res = !SET_WIN_STYLE(style);
    // IDK show this shit works, so let's fuck Microsoft again
    res = !this->u32->SetWindowPos(
        this->hwnd, NULL, 0, 0, 0, 0,
        SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED
    ) || true;
    return res;
}

bool omg_window_win_set_bordered(OMG_WindowWin* this, bool enabled) {
    int64_t style = GET_WIN_STYLE();
    if (!style)
        return true;
    base->bordered = enabled;
    if (enabled) {
        style &= ~WS_POPUP;
        style |= WS_CAPTION;
    }
    else {
        style |= WS_POPUP;
        style &= ~WS_CAPTION;
    }
    return !SET_WIN_STYLE(style);
}

bool omg_window_win_set_always_on_top(OMG_WindowWin* this, bool enabled) {
    int64_t style = GET_WIN_STYLE();
    if (!style)
        return true;
    base->always_on_top = enabled;
    // TODO
    return !SET_WIN_STYLE(style);
}

bool omg_window_win_renderer_alloc(OMG_WindowWin* this) {
    if (base->ren_type != OMG_REN_TYPE_SDL2)
        base->ren_type = OMG_REN_TYPE_AUTO;
    if (base->ren_type == OMG_REN_TYPE_AUTO) {
        base->ren_type = OMG_SUPPORT_SDL2 ? OMG_REN_TYPE_SDL2 : OMG_REN_TYPE_NONE;
        if (base->ren_type == OMG_REN_TYPE_NONE)
            return true;
        bool res = omg_window_win_renderer_alloc(this);
        if (res) {
            base->ren_type = OMG_REN_TYPE_AUTO;
        }
        return res;
    }
#if OMG_SUPPORT_SDL2
    if (base->ren_type == OMG_REN_TYPE_SDL2) {
        base->ren = OMG_MALLOC(omg_base->mem, sizeof(OMG_RendererSdl2) + sizeof(OMG_Sdl2));
        if (OMG_ISNULL(base->ren))
            return true;
        ren_sdl2->sdl2 = (void*)((size_t)base->ren + sizeof(OMG_RendererSdl2));
        if (omg_sdl2_dll_load(ren_sdl2->sdl2, omg_base->sdl2_dll_path)) {
            OMG_FREE(omg_base->mem, base->ren);
            base->ren = NULL;
            return true;
        }
        omg_renderer_fill_on_create(base->ren);
        ren_sdl2->id_cache[0] = -1;
        ren_sdl2->win_build_num = this->win_build_num;
        base->ren->was_allocated = true;
        base->ren->win = this;
        base->ren->omg = omg_base;
        OMG_BEGIN_POINTER_CAST();
        base->ren->init = omg_renderer_sdl2_init;
        base->ren->get_supported_drivers = omg_renderer_sdl2_get_supported_drivers;
        OMG_END_POINTER_CAST();
        if (ren_sdl2->sdl2->SDL_Init(SDL_INIT_VIDEO) < 0) {
            omg_sdl2_dll_free(ren_sdl2->sdl2);
            OMG_FREE(omg_base->mem, base->ren);
            base->ren = NULL;
            return true;
        }
        ren_sdl2->win = ren_sdl2->sdl2->SDL_CreateWindowFrom((const void*)this->hwnd);
        if (OMG_ISNULL(ren_sdl2->win)) {
            ren_sdl2->sdl2->SDL_Quit();
            omg_sdl2_dll_free(ren_sdl2->sdl2);
            OMG_FREE(omg_base->mem, base->ren);
            base->ren = NULL;
            return true;
        }
        return false;
    }
#endif
    return true;
}

bool omg_window_win_renderer_free(OMG_WindowWin* this) {
    if (OMG_ISNULL(base->ren))
        return true;
    bool res = base->ren->destroy(base->ren);
#if OMG_SUPPORT_SDL2
    if (base->ren->type == OMG_REN_TYPE_SDL2) {
        if (OMG_ISNOTNULL(base->ren)) {
            ren_sdl2->sdl2->SDL_DestroyWindow(ren_sdl2->win);
            ren_sdl2->sdl2->SDL_Quit();
            res = omg_sdl2_dll_free(ren_sdl2->sdl2) || res;
            ren_sdl2->sdl2 = NULL;
        }
    }
#endif
    omg_window_renderer_free((OMG_Window*)this);
    return res;
}

LRESULT omg_win_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
#ifdef SetWindowLongPtrW
    OMG_WindowWin* this = (OMG_WindowWin*)OMG_WIN_CB_GetWindowLongW(hwnd, GWLP_USERDATA);
#else
    OMG_WindowWin* this = (OMG_WindowWin*)OMG_WIN_CB_GetWindowLongPtrW(hwnd, GWLP_USERDATA);
#endif
    switch (msg) {
        case WM_PAINT: {
            OMG_EventPaint p_event;
            MAKE_EVENT(&p_event);
            p_event.win = this;
            omg_base->on_paint(&p_event);
            return RET_DEF_PROC();
        }
        case WM_WINDOWPOSCHANGED: {
            WINDOWPOS* pos = (WINDOWPOS*)lparam;
            OMG_UNUSED(pos);
            return RET_DEF_PROC();
        }
        case WM_WINDOWPOSCHANGING: {
            WINDOWPOS* pos = (WINDOWPOS*)lparam;
            if (!base->resizable) { // Why??? Fuck Microsoft!
                pos->cx = (int)this->size_cache.w;
                pos->cy = (int)this->size_cache.h;
                pos->flags |= SWP_NOSIZE;
                return FALSE;
            }
            return RET_DEF_PROC();
        }
        case WM_SIZING: {
            RECT* new_rect = (RECT*)lparam;
            if (!base->resizable) {
                new_rect->right = new_rect->left + (LONG)this->size_cache.w;
                new_rect->bottom = new_rect->top + (LONG)this->size_cache.h;
                return TRUE;
            }
            return TRUE;
        }
        case WM_SIZE: {
            UINT new_w = (UINT)LOWORD(lparam);
            UINT new_h = (UINT)HIWORD(lparam);
            if ((wparam == SIZE_MINIMIZED) && !(base->state & OMG_WIN_STATE_MINIMIZED)) {
                OMG_EventStateChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.change = OMG_WIN_STATE_MINIMIZED;
                event.prev_state = base->state;
                base->state &= ~OMG_WIN_STATE_RESTORED;
                base->state |= OMG_WIN_STATE_MINIMIZED;
                omg_base->on_state_change(&event);
            }
            if ((wparam == SIZE_MAXIMIZED) && !(base->state & OMG_WIN_STATE_MAXIMIZED)) {
                OMG_EventStateChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.change = OMG_WIN_STATE_MAXIMIZED;
                event.prev_state = base->state;
                base->state &= ~OMG_WIN_STATE_RESTORED;
                base->state |= OMG_WIN_STATE_MAXIMIZED;
                omg_base->on_state_change(&event);
            }
            if ((wparam != SIZE_MAXIMIZED) && (base->state & OMG_WIN_STATE_MAXIMIZED)) {
                OMG_EventStateChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.change = OMG_WIN_STATE_RESTORED;
                event.prev_state = base->state;
                base->state &= ~OMG_WIN_STATE_MAXIMIZED;
                base->state |= OMG_WIN_STATE_RESTORED;
                omg_base->on_state_change(&event);
            }
            if ((wparam != SIZE_MINIMIZED) && (base->state & OMG_WIN_STATE_MINIMIZED)) {
                OMG_EventStateChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.change = OMG_WIN_STATE_RESTORED;
                event.prev_state = base->state;
                base->state &= ~OMG_WIN_STATE_MINIMIZED;
                if (!(base->state & OMG_WIN_STATE_MAXIMIZED))
                    base->state |= OMG_WIN_STATE_RESTORED;
                omg_base->on_state_change(&event);
            }
            if (base->resizable && (new_w > 0) && (new_h > 0) && ((new_w != (UINT)base->size.w) || (new_h != (UINT)base->size.h))) {
                OMG_EventResize event;
                MAKE_EVENT(&event);
                event.win = this;
                base->size.w = event.size.w = (float)new_w;
                base->size.h = event.size.h = (float)new_h;
                if (OMG_ISNOTNULL(base->ren))
                    base->ren->_on_update_window_size(base->ren);
                omg_base->on_size_change(&event);
                omg_base->on_resize(&event);
            }
            return RET_DEF_PROC();
        }
        case WM_SYSCOMMAND: {
            if ((wparam == SC_VSCROLL) || (wparam == SC_HSCROLL) || (wparam == SC_KEYMENU))
                return FALSE;
            else if (wparam == SC_CLOSE) {
                if (!(base->sys_buttons & OMG_WIN_SYS_BUTTON_CLOSE))
                    return FALSE;
            }
            else if (wparam == SC_MAXIMIZE) {
                if (!(base->sys_buttons & OMG_WIN_SYS_BUTTON_MAXIMIZE))
                    return FALSE;
            }
            else if (wparam == SC_MINIMIZE) {
                if (!(base->sys_buttons & OMG_WIN_SYS_BUTTON_MINIMIZE))
                    return FALSE;
            }
            return RET_DEF_PROC();
        }
        case WM_THEMECHANGED: {
            omg_window_win_check_dark_mode(this);
            return RET_DEF_PROC();
        }
        case WM_NCCALCSIZE: {
            omg_window_win_update_scale(this);
            return RET_DEF_PROC();
        }
        case WM_DESTROY: {
            // TODO: it's not really quit, but should work for atleast one window
            // TODO: fix this quit shit
            OMG_EventQuit event;
            MAKE_EVENT(&event);
            omg_base->on_quit(&event);
            //this->u32->PostQuitMessage(0);
            return RET_DEF_PROC();
        }
        case WM_QUIT: {
            // Why???
            /*OMG_EventQuit event;
            MAKE_EVENT(&event);
            omg_base->on_quit(&event);*/
            return 0;
        }
        case WM_NCCREATE: {
            LPCREATESTRUCTW lps = (LPCREATESTRUCTW)lparam;
            this = (OMG_WindowWin*)lps->lpCreateParams;
            if (OMG_ISNULL(this->u32->SetWindowLongPtrW))
                this->u32->SetWindowLongW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
            else
                this->u32->SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
            if (omg_base->support_highdpi && OMG_ISNOTNULL(this->u32->EnableNonClientDpiScaling))
                this->u32->EnableNonClientDpiScaling(hwnd);
            return RET_DEF_PROC();
        }
        default: {
            if (OMG_ISNOTNULL(this)) {
                // _OMG_LOG_WARN(omg_base, "TODO Event: ", (int)msg);
                return RET_DEF_PROC();
            }
            return 0;
        }
    }
    if (OMG_ISNOTNULL(this))
        return RET_DEF_PROC();
    else
        return 0;
}

bool omg_window_win_destroy(OMG_WindowWin* this) {
    bool result = false;
    if (base->inited) {
        omg_window_destroy((OMG_Window*)this);
        if (!this->u32->DestroyWindow(this->hwnd)) {
            // _OMG_LOG_WARN(omg_base, "Failed to destroy window");
            result = true;
        }
        if (!this->u32->UnregisterClassW(this->wc.lpszClassName, this->wc.hInstance)) {
            _OMG_LOG_WARN(omg_base, "Failed to unregister class");
            result = true;
        }
        base->inited = false;
    }
    return result;
}

void omg_window_win_update_scale(OMG_WindowWin* this) {
    OMG_FPoint new_scale;
    if (omg_base->support_highdpi) {
        if (OMG_ISNULL(this->u32->GetDpiForWindow)) {
            HDC hdc = this->u32->GetWindowDC(this->hwnd);
            new_scale.x = (float)this->g32->GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
            new_scale.y = (float)this->g32->GetDeviceCaps(hdc, LOGPIXELSY) / 96.0f;
            this->u32->ReleaseDC(this->hwnd, hdc);
        }
        else {
            new_scale.x = new_scale.y = (float)this->u32->GetDpiForWindow(this->hwnd) / 96.0f;
        }
    }
    else {
        new_scale.x = new_scale.y = 1.0f;
    }
    if (new_scale.x == base->scale.x && new_scale.y == base->scale.y)
        return;
    if (base->scale.x == 0.0f)
        base->scale.x = 1.0f;
    RECT desktop_rect;
    RECT rect;
    RECT c_rect;
    HWND hwnd_d = this->u32->GetDesktopWindow();
    if (!this->u32->GetClientRect(hwnd_d, &desktop_rect) || !this->u32->GetWindowRect(this->hwnd, &rect) || !this->u32->GetClientRect(this->hwnd, &c_rect))
        return;
    /*
    float new_w = (float)(rect.right - rect.left) * new_scale.x / base->scale.x;
    float new_h = (float)(rect.bottom - rect.top) * new_scale.y / base->scale.y;
    */
    this->size_cache.w = base->size.w * new_scale.x + (float)(rect.right - rect.left - c_rect.right);
    this->size_cache.h = base->size.h * new_scale.y + (float)(rect.bottom - rect.top - c_rect.bottom);
    /*this->u32->MoveWindow(
        this->hwnd,
        base->centered ? (int)(((float)desktop_rect.right - this->size_cache.w) / 2.0f) : rect.right,
        base->centered ? (int)(((float)desktop_rect.bottom - this->size_cache.h) / 2.0f) : rect.bottom,
        (int)this->size_cache.w,
        (int)this->size_cache.h,
        TRUE
    );*/
    this->u32->SetWindowPos(
        this->hwnd,
        NULL,
        (int)(((float)desktop_rect.right - this->size_cache.w) / 2.0f),
        (int)(((float)desktop_rect.bottom - this->size_cache.h) / 2.0f),
        (int)this->size_cache.w, (int)this->size_cache.h,
        SWP_NOZORDER | SWP_NOACTIVATE | (base->centered ? 0 : SWP_NOMOVE) | SWP_FRAMECHANGED
    );
}

bool omg_window_win_init(OMG_WindowWin* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_WIN;
    base->inited = false;
    this->wc.cbSize = sizeof(WNDCLASSEXW);
    this->wc.style = CS_HREDRAW | CS_VREDRAW;
    this->wc.lpfnWndProc = (WNDPROC)omg_win_wnd_proc;
    this->wc.cbClsExtra = 0;
    this->wc.cbWndExtra = 0;
#if _OMG_ENTRY_DATA_TYPE == 2
    this->wc.hInstance = omg_base->entry_data->hInst;
#else
    this->wc.hInstance = (HINSTANCE)this->k32->GetModuleHandleW(NULL);
#endif
    this->wc.hIcon = NULL;
    this->wc.hCursor = (HCURSOR)this->u32->LoadImageW(
        NULL, (LPCWSTR)IDC_ARROW,
        IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED
    );
    this->wc.hbrBackground = this->u32->GetSysColorBrush(COLOR_3DFACE);
    this->wc.lpszMenuName = NULL;
    this->wc.lpszClassName = L"OMG_App"; // TODO: custom name to support multiple windows
    this->wc.hIconSm = NULL;
    if (OMG_ISNOTNULL(this->u32->GetWindowLongPtrW))
        OMG_WIN_CB_GetWindowLongPtrW = this->u32->GetWindowLongPtrW;
    if (OMG_ISNOTNULL(this->u32->GetWindowLongW))
        OMG_WIN_CB_GetWindowLongW = this->u32->GetWindowLongW;
    this->class_atom = this->u32->RegisterClassExW(&this->wc);
    if (!this->class_atom) {
        DWORD error = this->k32->GetLastError();
        DWORD res;
        wchar_t* w_error_buffer;
        _OMG_WIN_FORMAT_ERROR(omg_base->mem, this->k32, error, w_error_buffer, res);
        if (res > 2) {
            w_error_buffer[res - 3] = L'\0';
            _OMG_LOG_ERROR(omg_base, "Failed to register class atom (", w_error_buffer, ")");
        }
        else
            _OMG_LOG_ERROR(omg_base, "Failed to register class atom (Error Code - ", error, ")");
        if (OMG_ISNOTNULL(w_error_buffer))
            OMG_FREE(omg_base->mem, w_error_buffer);
        return true;
    }
    this->hwnd = this->u32->CreateWindowExW(
        WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOINHERITLAYOUT,
        this->wc.lpszClassName, L"OMG Window [Win32]",
        (base->bordered ? WS_CAPTION : WS_POPUP) |
        ((base->sys_buttons & OMG_WIN_SYS_BUTTON_CLOSE) ? WS_SYSMENU : 0) | \
        ((base->sys_buttons & OMG_WIN_SYS_BUTTON_MINIMIZE) ? WS_MINIMIZEBOX : 0) | \
        ((base->sys_buttons & OMG_WIN_SYS_BUTTON_MAXIMIZE) ? WS_MAXIMIZEBOX : 0) | \
        ((base->state & OMG_WIN_STATE_MAXIMIZED) ? WS_MAXIMIZE : 0) | \
        ((base->state & OMG_WIN_STATE_MINIMIZED) ? WS_MINIMIZE : 0) | \
        (base->resizable ? WS_THICKFRAME : 0) | \
        WS_OVERLAPPED,
        CW_USEDEFAULT, CW_USEDEFAULT,
        (int)(base->size.w * base->scale.x) + 31, (int)(base->size.h * base->scale.y) + 16,
        NULL, NULL, this->wc.hInstance, this
    );
    if (OMG_ISNULL(this->hwnd)) {
        DWORD error = this->k32->GetLastError();
        DWORD res;
        wchar_t* w_error_buffer;
        _OMG_WIN_FORMAT_ERROR(omg_base->mem, this->k32, error, w_error_buffer, res);
        if (res > 2) {
            w_error_buffer[res - 3] = L'\0';
            _OMG_LOG_ERROR(omg_base, "Failed to create window (", w_error_buffer, ")");
        }
        else
            _OMG_LOG_ERROR(omg_base, "Failed to create window (Error Code - ", error, ")");
        if (OMG_ISNOTNULL(w_error_buffer))
            OMG_FREE(omg_base->mem, w_error_buffer);
        this->u32->UnregisterClassW(this->wc.lpszClassName, this->wc.hInstance);
        return true;
    }
    omg_window_win_check_dark_mode(this);
    base->scale.x = 0.0f; // Hack
    omg_window_win_update_scale(this);
    OMG_BEGIN_POINTER_CAST();
    base->set_state = omg_window_win_set_state;
    base->set_sys_button = omg_window_win_set_sys_button;
    base->set_resizable = omg_window_win_set_resizable;
    base->set_bordered = omg_window_win_set_bordered;
    base->set_always_on_top = omg_window_win_set_always_on_top;
    base->show = omg_window_win_show;
    base->set_title = omg_window_win_set_title;
    base->destroy = omg_window_win_destroy;
    base->renderer_alloc = omg_window_win_renderer_alloc;
    base->renderer_free = omg_window_win_renderer_free;
    OMG_END_POINTER_CAST();
    base->inited = true;
    _OMG_LOG_INFO(omg_base, "Win32 window created successfuly");
    return false;
}
#endif
