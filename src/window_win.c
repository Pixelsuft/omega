#include <omega/window_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega.h>
#include <omega/renderer_sdl2.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define ren_sdl2 ((OMG_RendererSdl2*)base->ren)
#define RET_DEF_PROC() this->u32->DefWindowProcW(hwnd, msg, wparam, lparam)
#define MAKE_EVENT(event) do { \
    ((OMG_Event*)event)->omg = base->omg; \
    ((OMG_Event*)event)->data = omg_base->event_arg; \
    ((OMG_Event*)event)->time = 0; \
} while (0)

bool omg_window_win_show(OMG_WindowWin* this, bool show) {
    this->u32->ShowWindow(this->hwnd, show ? OMG_WIN_SW_SHOWNORMAL : OMG_WIN_SW_HIDE);
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
    wchar_t* out_buf = OMG_MALLOC(omg_base->mem, (size_t)count * 2);
    if (OMG_ISNULL(out_buf))
        return true;
    bool result = true;
    if (this->k32->MultiByteToWideChar(OMG_WIN_CP_UTF8, 0, new_title->ptr, new_title->len, out_buf, (int)count) > 0) {
        result = !this->u32->SetWindowTextW(this->hwnd, out_buf);
    }
    OMG_FREE(omg_base->mem, out_buf);
    return result;
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
            return true;
        }
        base->ren->was_allocated = true;
        base->ren->win = this;
        base->ren->omg = omg_base;
        OMG_BEGIN_POINTER_CAST();
        base->ren->init = omg_renderer_sdl2_init;
        OMG_END_POINTER_CAST();
        if (ren_sdl2->sdl2->SDL_Init(SDL_INIT_VIDEO) < 0) {
            // TODO: cleanup
            return true;
        }
        ren_sdl2->win = ren_sdl2->sdl2->SDL_CreateWindowFrom((const void*)this->hwnd);
        if (OMG_ISNULL(ren_sdl2->win)) {
            // TODO: cleanup
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
    OMG_WindowWin* this = (OMG_WindowWin*)OMG_WIN_CB_GetWindowLongW(hwnd, OMG_WIN_GWLP_USERDATA);
#else
    OMG_WindowWin* this = (OMG_WindowWin*)OMG_WIN_CB_GetWindowLongPtrW(hwnd, OMG_WIN_GWLP_USERDATA);
#endif
    switch (msg) {
        case OMG_WIN_WM_NCCREATE: {
            OMG_WIN_LPCREATESTRUCTW lps = (OMG_WIN_LPCREATESTRUCTW)lparam;
            this = (OMG_WindowWin*)lps->lpCreateParams;
            if (OMG_ISNULL(this->u32->SetWindowLongPtrW))
                this->u32->SetWindowLongW(hwnd, OMG_WIN_GWLP_USERDATA, (LONG_PTR)this);
            else
                this->u32->SetWindowLongPtrW(hwnd, OMG_WIN_GWLP_USERDATA, (LONG_PTR)this);
            return RET_DEF_PROC();
        }
        case OMG_WIN_WM_THEMECHANGED: {
            omg_window_win_check_dark_mode(this);
            return RET_DEF_PROC();
        }
        case OMG_WIN_WM_DESTROY: {
            // TODO: it's not really quit, but should work for atleast one window
            OMG_EventQuit event;
            MAKE_EVENT(&event);
            omg_base->on_quit(&event);
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
            _OMG_LOG_WARN(omg_base, "Failed to destroy window");
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

bool omg_window_win_init(OMG_WindowWin* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_WIN;
    base->inited = false;
    this->wc.cbSize = sizeof(OMG_WIN_WNDCLASSEXW);
    this->wc.style = OMG_WIN_CS_HREDRAW | OMG_WIN_CS_VREDRAW;
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
        NULL, (LPCWSTR)OMG_WIN_IDC_ARROW,
        OMG_WIN_IMAGE_CURSOR, 0, 0, OMG_WIN_LR_DEFAULTSIZE | OMG_WIN_LR_SHARED
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
        WS_OVERLAPPEDWINDOW, // TODO: customize
        CW_USEDEFAULT, CW_USEDEFAULT,
        (int)base->size.w, (int)base->size.h,
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
    OMG_BEGIN_POINTER_CAST();
    base->show = omg_window_win_show;
    base->set_title = omg_window_win_set_title;
    base->destroy = omg_window_destroy;
    base->renderer_alloc = omg_window_win_renderer_alloc;
    base->renderer_free = omg_window_win_renderer_free;
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}
#endif
