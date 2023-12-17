#include <omega/window_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_window_win_show(OMG_WindowWin* this) {
    OMG_UNUSED(this);
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
    this->wc.hInstance = (HINSTANCE)this->k32->GetModuleHandleW(NULL);
    this->wc.hIcon = NULL;
    this->wc.hCursor = NULL;
    this->wc.hbrBackground = NULL;
    this->wc.lpszMenuName = NULL;
    this->wc.lpszClassName = L"OMG_App";
    this->wc.hIconSm = NULL;
    base->inited = true;
    OMG_BEGIN_POINTER_CAST();
    base->show = omg_window_win_show;
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_window_win_destroy(OMG_WindowWin* this) {
    OMG_UNUSED(this);
    if (base->inited) {
        base->inited = false;
    }
    return false;
}
#endif
