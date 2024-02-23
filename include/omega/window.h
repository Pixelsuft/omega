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
    /* Init window */
    bool (*default_init)(struct OMG_Window* this);
    /* Destroy window */
    bool (*destroy)(struct OMG_Window* this);
    /* Set window shown */
    bool (*show)(struct OMG_Window* this, bool show);
    bool (*set_title)(struct OMG_Window* this, const OMG_String* new_title);
    bool (*set_size)(struct OMG_Window* this, const OMG_FPoint* new_size);
    bool (*set_min_size)(struct OMG_Window* this, const OMG_FPoint* new_min_size);
    bool (*set_max_size)(struct OMG_Window* this, const OMG_FPoint* new_max_size);
    /* Alloc renderer for window */
    bool (*renderer_alloc)(struct OMG_Window* this);
    /* Free allocated renderer */
    bool (*renderer_free)(struct OMG_Window* this);
    /* Minimize, maximize or restore window */
    bool (*set_state)(struct OMG_Window* this, int state);
    /* Set the first Z-order */
    bool (*raise)(struct OMG_Window* this);
    /* Go fullscreen or windowed */
    bool (*set_window_mode)(struct OMG_Window* this, int mode);
    bool (*set_sys_button)(struct OMG_Window* this, int id, bool enabled);
    bool (*set_resizable)(struct OMG_Window* this, bool enabled);
    /* Set window border enabled */
    bool (*set_bordered)(struct OMG_Window* this, bool enabled);
    bool (*set_always_on_top)(struct OMG_Window* this, bool enabled);
    bool (*set_icon)(struct OMG_Window* this, OMG_Surface* icon);
    /* Set mouse pos */
    bool (*mouse_warp)(struct OMG_Window* this, const OMG_FPoint* pos);
    /* Set mouse relative mode. 0 - off, 1 - on, 2 - toggle */
    bool (*mouse_set_rel)(struct OMG_Window* this, int rel_mode);
    /* Set cursor shown. 0 - off, 1 - on, 2 - toggle */
    bool (*cursor_set_shown)(struct OMG_Window* this, int show_mode);
    /* Set cursor from system cursors */
    bool (*mouse_set_system_cursor)(struct OMG_Window* this, int cursor_id);
    /* Set cursor rect */
    bool (*mouse_set_rect)(struct OMG_Window* this, const OMG_FRect* rect);
    /* Set grabbed. 0 - off, 1 - on, 2 - toggle */
    bool (*set_grab)(struct OMG_Window* this, int grab_mode);
    /* Get cursor pos */
    bool (*get_pos)(struct OMG_Window* this, OMG_FRect* pos);
    /* Set cursor pos */
    bool (*set_pos)(struct OMG_Window* this, const OMG_FRect* pos);
    /* Get window display mode */
    bool (*display_get_mode)(struct OMG_Window* this, OMG_VideoMode* mode);
    /* Set window display mode */
    bool (*display_set_mode)(struct OMG_Window* this, const OMG_VideoMode* mode);
    /* Get window display index */
    int (*display_get_index)(struct OMG_Window* this);
    bool (*set_brightness)(struct OMG_Window* this, float brightness);
    float (*get_brightness)(struct OMG_Window* this);
    /* Set alpha blend value */
    bool (*set_opacity)(struct OMG_Window* this, float opacity);
    /* Get alpha blend value */
    float (*get_opacity)(struct OMG_Window* this);
    /* Show simple message box for window */
    bool (*message_box)(struct OMG_Window* this, const OMG_String* text, const OMG_String* title, int flags);
    OMG_Renderer* ren;
    void* extra1;
    void* extra2;
    void* extra3;
    void* extra4;
    void* extra5;
    void* omg;
    void* win32_handle;
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
OMG_API bool omg_window_display_set_mode(OMG_Window* this, const OMG_VideoMode* mode);
OMG_API bool omg_window_get_pos(OMG_Window* this, OMG_FRect* pos);
OMG_API bool omg_window_set_pos(OMG_Window* this, const OMG_FRect* pos);
OMG_API bool omg_window_mouse_set_rect(OMG_Window* this, const OMG_FRect* rect);
OMG_API bool omg_window_raise(OMG_Window* this);
OMG_API bool omg_window_set_opacity(OMG_Window* this, float opacity);
OMG_API float omg_window_get_opacity(OMG_Window* this);
OMG_API bool omg_window_set_brightness(OMG_Window* this, float brightness);
OMG_API float omg_window_get_brightness(OMG_Window* this);
OMG_API void omg_window_win_check_dark_mode(OMG_Window* this);
OMG_API int omg_window_display_get_index(OMG_Window* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_window_message_box(OMG_Window* this, const OMG_String* text, const OMG_String* title, int flags);
OMG_API bool omg_window_set_state(OMG_Window* this, int state);
OMG_API bool omg_window_set_window_mode(OMG_Window* this, int mode);
OMG_API bool omg_window_set_sys_button(OMG_Window* this, int id, bool enabled);
OMG_API bool omg_window_set_resizable(OMG_Window* this, bool enabled);
OMG_API bool omg_window_set_bordered(OMG_Window* this, bool enabled);
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
