#pragma once
#include <omega/config.h>
#include <omega/window.h>

#if OMG_SUPPORT_WIN
#include <omega/api_win.h>

typedef struct {
    OMG_Window parent;
    OMG_WIN_WNDCLASSEXW wc;
    HWND hwnd;
    OMG_User32* u32;
    void* omg;
} OMG_WindowWin;

OMG_API bool omg_window_win_init(OMG_WindowWin* this);
OMG_API bool omg_window_win_destroy(OMG_WindowWin* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_window_win_show(OMG_WindowWin* this);
#endif
#endif
