#pragma once
#include <omega/config.h>
#include <omega/window.h>

#if OMG_SUPPORT_WIN
#include <omega/api_win.h>

typedef struct {
    OMG_Window parent;
    WNDCLASSEXW wc;
    ATOM class_atom;
    HWND hwnd;
    OMG_User32* u32;
    OMG_Gdi32* g32;
    OMG_Kernel32* k32;
    OMG_Dwmapi* dwm;
    OMG_Uxtheme* uxtheme;
    OMG_FPoint size_cache;
    OMG_Point mouse_pos_cache;
    int win_build_num;
    bool destroyed;
} OMG_WindowWin;

OMG_API bool omg_window_win_init(OMG_WindowWin* this);
OMG_API bool omg_window_win_destroy(OMG_WindowWin* this);
OMG_API LRESULT omg_win_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
OMG_API void omg_window_win_update_scale(OMG_WindowWin* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_window_win_set_state(OMG_WindowWin* this, int state);
OMG_API bool omg_window_win_set_sys_button(OMG_WindowWin* this, int id, bool enabled);
OMG_API bool omg_window_win_set_resizable(OMG_WindowWin* this, bool enabled);
OMG_API bool omg_window_win_set_bordered(OMG_WindowWin* this, bool enabled);
OMG_API bool omg_window_win_set_always_on_top(OMG_WindowWin* this, bool enabled);
OMG_API void omg_window_win_check_dark_mode(OMG_WindowWin* this);
OMG_API bool omg_window_win_show(OMG_WindowWin* this, bool show);
OMG_API bool omg_window_win_set_title(OMG_WindowWin* this, const OMG_String* new_title);
OMG_API bool omg_window_win_renderer_alloc(OMG_WindowWin* this);
OMG_API bool omg_window_win_renderer_free(OMG_WindowWin* this);
#endif
#endif
