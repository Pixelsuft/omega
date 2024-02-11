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
    RECT clip_rect;
    HDC hdc;
    WPARAM last_mouse_state;
    OMG_FPoint size_cache;
    OMG_Point mouse_pos_cache;
    uint32_t mouse_state_cache;
    WCHAR high_surrogate;
    UINT resize_timer;
    int win_build_num;
    bool showing_cursor;
    bool is_mouse_rel;
    bool is_mouse_left;
    bool is_focused;
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
OMG_API bool omg_window_win_mouse_warp(OMG_WindowWin* this, const OMG_FPoint* pos);
OMG_API bool omg_window_win_mouse_set_system_cursor(OMG_WindowWin* this, int cursor_id);
OMG_API bool omg_window_win_cursor_set_shown(OMG_WindowWin* this, int show_mode);
OMG_API bool omg_window_win_mouse_set_rel(OMG_WindowWin* this, int rel_mode);
OMG_API bool omg_window_win_set_grab(OMG_WindowWin* this, int grab_mode);
OMG_API bool omg_window_win_set_pos(OMG_WindowWin* this, const OMG_FRect* pos);
OMG_API bool omg_window_win_set_size(OMG_WindowWin* this, const OMG_FRect* new_size);
OMG_API bool omg_window_win_get_pos(OMG_WindowWin* this, OMG_FRect* pos);
#endif
#endif
