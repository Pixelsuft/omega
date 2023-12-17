#pragma once
#include <omega/config.h>
#include <omega/window.h>

#if OMG_SUPPORT_WIN
#include <omega/api_win.h>

typedef struct {
    OMG_Window parent;
    OMG_WIN_WNDCLASSEXW wc;
    ATOM class_atom;
    HWND hwnd;
    OMG_User32* u32;
    OMG_Kernel32* k32;
    OMG_Dwmapi* dwm;
    OMG_Uxtheme* uxtheme;
    void* omg;
} OMG_WindowWin;

OMG_API bool omg_window_win_init(OMG_WindowWin* this);
OMG_API bool omg_window_win_destroy(OMG_WindowWin* this);
#if OMG_EXPORT_SHIT
OMG_API void omg_window_win_check_dark_mode(OMG_WindowWin* this);
OMG_API bool omg_window_win_show(OMG_WindowWin* this);
#endif
#endif
