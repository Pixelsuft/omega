#include <omega/window_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_window_win_show(OMG_WindowWin* this) {
    this->u32->ShowWindow(this->hwnd, OMG_WIN_SW_SHOWNORMAL);
    return false;
}

bool omg_window_win_init(OMG_WindowWin* this) {
    omg_window_init(base);
    base->inited = false;
    this->wc.cbSize = sizeof(OMG_WIN_WNDCLASSEXW);
    this->wc.style = 0;
    this->wc.lpfnWndProc = omg_win_wnd_proc;
    this->wc.cbClsExtra = 0;
    this->wc.cbWndExtra = 0;
    this->wc.hInstance = (HINSTANCE)this->k32->GetModuleHandleW(NULL); // TODO: get from OMG_EntryData
    this->wc.hIcon = NULL;
    this->wc.hCursor = (HCURSOR)this->u32->LoadImageW(
        NULL, (LPCWSTR)OMG_WIN_IDC_ARROW,
        OMG_WIN_IMAGE_CURSOR, 0, 0, OMG_WIN_LR_DEFAULTSIZE | OMG_WIN_LR_SHARED
    );
    this->wc.hbrBackground = NULL;
    this->wc.lpszMenuName = NULL;
    this->wc.lpszClassName = L"OMG_App"; // TODO: custom name to support multiple windows
    this->wc.hIconSm = NULL;
    this->class_atom = this->u32->RegisterClassExW(&this->wc);
    if (!this->class_atom) {
        DWORD error = this->k32->GetLastError();
        DWORD res;
        wchar_t* w_error_buffer;
        _OMG_WIN_FORMAT_ERROR(omg_base->mem, this->k32, error, w_error_buffer, res);
        if ((omg_base->type == OMG_OMEGA_TYPE_WIN) && (res > 2)) {
            w_error_buffer[res - 3] = L'\0';
            _OMG_LOG_ERROR(omg_base, "Failed to register class atom (", w_error_buffer, ")");
        }
        else
            _OMG_LOG_ERROR(omg_base, "Failed to register class atom (Error Code - ", error, ")");
        if (OMG_ISNOTNULL(w_error_buffer))
            OMG_FREE(omg_base->mem, w_error_buffer);
        return true;
    }
    base->inited = true;
    OMG_BEGIN_POINTER_CAST();
    base->show = omg_window_win_show;
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_window_win_destroy(OMG_WindowWin* this) {
    bool result = false;
    if (base->inited) {
        if (!this->u32->UnregisterClassW(this->wc.lpszClassName, this->wc.hInstance)) {
            _OMG_LOG_WARN(omg_base, "Failed to unregister class");
            result = true;
        }
        base->inited = false;
    }
    return result;
}
#endif
