#pragma once
#include <omega/ostd.h>
#include <omega/events.h>
#include <omega/renderer.h>
#include <omega/display.h>

#define OMG_WIN_TYPE_NONE 0
#define OMG_WIN_TYPE_WIN 1
#define OMG_WIN_TYPE_SDL2 2
#define OMG_WIN_TYPE_RAYLIB 3

#define OMG_WIN_SYS_BUTTON_CLOSE (1 << 1)
#define OMG_WIN_SYS_BUTTON_MAXIMIZE (1 << 2)
#define OMG_WIN_SYS_BUTTON_MINIMIZE (1 << 3)

#define OMG_WIN_MODE_WINDOW 0
#define OMG_WIN_MODE_FULLSCREEN 1
#define OMG_WIN_MODE_DESKTOP_FULLSCREEN 2

#define OMG_WIN_STATE_RESTORED (1 << 1)
#define OMG_WIN_STATE_MINIMIZED (1 << 2)
#define OMG_WIN_STATE_MAXIMIZED (1 << 3)
#define OMG_WIN_STATE_CLOSED (1 << 4)

typedef struct OMG_Window {
    bool (*default_init)(struct OMG_Window* this);
    bool (*destroy)(struct OMG_Window* this);
    bool (*show)(struct OMG_Window* this, bool show);
    bool (*set_title)(struct OMG_Window* this, const OMG_String* new_title);
    bool (*set_size)(struct OMG_Window* this, const OMG_FPoint* new_size);
    bool (*set_min_size)(struct OMG_Window* this, const OMG_FPoint* new_min_size);
    bool (*set_max_size)(struct OMG_Window* this, const OMG_FPoint* new_max_size);
    bool (*renderer_alloc)(struct OMG_Window* this);
    bool (*renderer_free)(struct OMG_Window* this);
    bool (*set_state)(struct OMG_Window* this, int state);
    bool (*set_window_mode)(struct OMG_Window* this, int mode);
    bool (*set_sys_button)(struct OMG_Window* this, int id, bool enabled);
    bool (*set_resizable)(struct OMG_Window* this, bool enabled);
    bool (*set_bordered)(struct OMG_Window* this, bool enabled);
    bool (*set_thick)(struct OMG_Window* this, bool enabled);
    bool (*set_always_on_top)(struct OMG_Window* this, bool enabled);
    bool (*set_icon)(struct OMG_Window* this, OMG_Surface* icon);
    bool (*mouse_warp)(struct OMG_Window* this, const OMG_FPoint* pos);
    bool (*mouse_set_rel)(struct OMG_Window* this, int rel_mode);
    bool (*cursor_set_shown)(struct OMG_Window* this, int show_mode);
    bool (*mouse_set_system_cursor)(struct OMG_Window* this, int cursor_id);
    bool (*set_grab)(struct OMG_Window* this, int grab_mode);
    bool (*display_get_mode)(struct OMG_Window* this, OMG_VideoMode* mode);
    bool (*display_set_mode)(struct OMG_Window* this, OMG_VideoMode* mode);
    int (*display_get_index)(struct OMG_Window* this);
    OMG_Renderer* ren;
    void* extra1;
    void* extra2;
    void* extra3;
    void* extra4;
    void* extra5;
    void* omg;
    size_t array_pos;
    OMG_FPoint size;
    OMG_FPoint min_size;
    OMG_FPoint max_size;
    OMG_FPoint scale;
    int type;
    int state;
    int ren_type;
    int window_mode;
    int sys_buttons;
    bool vsync;
    bool resizable;
    bool thick; // Unused
    bool bordered;
    bool always_on_top;
    bool inited;
    bool centered;
    bool enable_paint;
    bool allow_alt;
    bool allow_alt_f4;
    bool aa;
    bool was_allocated;
} OMG_Window;

OMG_API bool omg_window_init(OMG_Window* this);
OMG_API void omg_window_fill_on_create(OMG_Window* this);
OMG_API bool omg_window_destroy(OMG_Window* this);
OMG_API bool omg_window_renderer_alloc(OMG_Window* this);
OMG_API bool omg_window_renderer_free(OMG_Window* this);
OMG_API bool omg_window_set_size(OMG_Window* this, const OMG_FPoint* new_size);
OMG_API bool omg_window_set_min_size(OMG_Window* this, const OMG_FPoint* new_min_size);
OMG_API bool omg_window_set_max_size(OMG_Window* this, const OMG_FPoint* new_max_size);
OMG_API bool omg_window_display_get_mode(OMG_Window* this, OMG_VideoMode* mode);
OMG_API bool omg_window_display_set_mode(OMG_Window* this, OMG_VideoMode* mode);
OMG_API int omg_window_display_get_index(OMG_Window* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_window_set_state(OMG_Window* this, int state);
OMG_API bool omg_window_set_window_mode(OMG_Window* this, int mode);
OMG_API bool omg_window_set_sys_button(OMG_Window* this, int id, bool enabled);
OMG_API bool omg_window_set_resizable(OMG_Window* this, bool enabled);
OMG_API bool omg_window_set_bordered(OMG_Window* this, bool enabled);
OMG_API bool omg_window_set_thick(OMG_Window* this, bool enabled);
OMG_API bool omg_window_set_always_on_top(OMG_Window* this, bool enabled);
OMG_API bool omg_window_set_title(OMG_Window* this, const OMG_String* new_title);
OMG_API bool omg_window_set_icon(OMG_Window* this, OMG_Surface* icon);
OMG_API bool omg_window_show(OMG_Window* this, bool show);
OMG_API bool omg_window_mouse_warp(OMG_Window* this, const OMG_FPoint* pos);
OMG_API bool omg_window_mouse_set_rel(OMG_Window* this, int rel_mode);
OMG_API bool omg_window_cursor_set_shown(OMG_Window* this, int show_mode);
OMG_API bool omg_window_mouse_set_system_cursor(OMG_Window* this, int cursor_id);
OMG_API bool omg_window_set_grab(OMG_Window* this, int grab_mode);
#endif
