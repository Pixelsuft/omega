#include <omega/window_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)this->omg)
#define RET_DEF_PROC() this->u32->DefWindowProcW(hwnd, msg, wparam, lparam);

bool omg_window_win_show(OMG_WindowWin* this) {
    this->u32->ShowWindow(this->hwnd, OMG_WIN_SW_SHOWNORMAL);
    return false;
}

static LONG_PTR OMG_WIN_STD_PREFIX (*OMG_WIN_CB_GetWindowLongPtrW)(HWND, int);

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
        DWORD val = (DWORD)omg_base->theme;
        this->dwm->DwmSetWindowAttribute(this->hwnd, 19, &val, sizeof(DWORD));
        val = (DWORD)omg_base->theme;
        this->dwm->DwmSetWindowAttribute(this->hwnd, 20, &val, sizeof(DWORD));
    }
}

LRESULT omg_win_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    OMG_WindowWin* this = (OMG_WindowWin*)OMG_WIN_CB_GetWindowLongPtrW(hwnd, OMG_WIN_GWLP_USERDATA);
    switch (msg) {
        case OMG_WIN_WM_NCCREATE: {
            OMG_WIN_LPCREATESTRUCTW lps = (OMG_WIN_LPCREATESTRUCTW)lparam;
            this = (OMG_WindowWin*)lps->lpCreateParams;
            this->u32->SetWindowLongPtrW(hwnd, OMG_WIN_GWLP_USERDATA, (LONG_PTR)this);
            return RET_DEF_PROC();
        }
        case OMG_WIN_WM_THEMECHANGED: {
            omg_window_win_check_dark_mode(this);
            return RET_DEF_PROC();
        }
        default: {
            if (OMG_ISNOTNULL(this)) {
                _OMG_LOG_WARN(omg_base, "TODO Event: ", (int)msg);
                return RET_DEF_PROC();
            }
        }
    }
    return 0;
}

bool omg_window_win_init(OMG_WindowWin* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_WIN;
    base->inited = false;
    this->wc.cbSize = sizeof(OMG_WIN_WNDCLASSEXW);
    this->wc.style = OMG_WIN_CS_HREDRAW | OMG_WIN_CS_VREDRAW;
    this->wc.lpfnWndProc = omg_win_wnd_proc;
    this->wc.cbClsExtra = 0;
    this->wc.cbWndExtra = 0;
    this->wc.hInstance = (HINSTANCE)this->k32->GetModuleHandleW(NULL); // TODO: get from OMG_EntryData
    this->wc.hIcon = NULL;
    this->wc.hCursor = (HCURSOR)this->u32->LoadImageW(
        NULL, (LPCWSTR)OMG_WIN_IDC_ARROW,
        OMG_WIN_IMAGE_CURSOR, 0, 0, OMG_WIN_LR_DEFAULTSIZE | OMG_WIN_LR_SHARED
    );
    this->wc.hbrBackground = this->u32->GetSysColorBrush(COLOR_3DFACE);
    this->wc.lpszMenuName = NULL;
    this->wc.lpszClassName = L"OMG_App"; // TODO: custom name to support multiple windows
    this->wc.hIconSm = NULL;
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
    if (OMG_ISNOTNULL(this->u32->GetWindowLongPtrW))
        OMG_WIN_CB_GetWindowLongPtrW = this->u32->GetWindowLongPtrW;
    this->hwnd = this->u32->CreateWindowExW(
        WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOINHERITLAYOUT,
        this->wc.lpszClassName, L"OMG Window",
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
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}

bool omg_window_win_destroy(OMG_WindowWin* this) {
    bool result = false;
    if (base->inited) {
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
#endif
