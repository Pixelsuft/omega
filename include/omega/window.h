#pragma once
#include <omega/ostd.h>
#include <omega/events.h>
#include <omega/renderer.h>

#define OMG_WIN_TYPE_NONE 0
#define OMG_WIN_TYPE_WIN 1
#define OMG_WIN_TYPE_SDL2 2
#define OMG_WIN_TYPE_RAYLIB 3

#define OMG_WIN_SYS_BUTTON_CLOSE 0
#define OMG_WIN_SYS_BUTTON_MAXIMIZE 1
#define OMG_WIN_SYS_BUTTON_MINIMIZE 2

#define OMG_WIN_MODE_WINDOW 0
#define OMG_WIN_MODE_FULLSCREEN 0
#define OMG_WIN_MODE_DESKTOP_FULLSCREEN 0

typedef struct OMG_Window {
    bool (*default_init)(struct OMG_Window* this);
    bool (*destroy)(struct OMG_Window* this);
    bool (*show)(struct OMG_Window* this, bool show);
    bool (*set_title)(struct OMG_Window* this, const OMG_String* new_title);
    bool (*renderer_alloc)(struct OMG_Window* this);
    bool (*renderer_free)(struct OMG_Window* this);
    OMG_Renderer* ren;
    void* extra1;
    void* extra2;
    void* extra3;
    void* extra4;
    void* extra5;
    void* omg;
    size_t array_pos;
    OMG_FPoint size;
    OMG_FPoint scale;
    int type;
    int ren_type;
    int window_mode;
    bool vsync;
    bool resizable;
    bool thick;
    bool has_border;
    bool sys_buttons[3];
    bool minimized;
    bool maximized;
    bool always_on_top;
    bool inited;
    bool centered;
    bool was_allocated;
} OMG_Window;

OMG_API bool omg_window_init(OMG_Window* this);
OMG_API void omg_window_fill_on_create(OMG_Window* this);
OMG_API bool omg_window_destroy(OMG_Window* this);
OMG_API bool omg_window_renderer_alloc(OMG_Window* this);
OMG_API bool omg_window_renderer_free(OMG_Window* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_window_set_title(OMG_Window* this, const OMG_String* new_title);
OMG_API bool omg_window_show(OMG_Window* this, bool show);
#endif
