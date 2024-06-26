#pragma once
#include <omega/window.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/api_raylib.h>

typedef struct {
    OMG_Window parent;
    OMG_Raylib* raylib;
    float opacity_cache;
} OMG_WindowRaylib;

OMG_API bool omg_window_raylib_init(OMG_WindowRaylib* this);
OMG_API bool omg_window_raylib_destroy(OMG_WindowRaylib* this);
#if OMG_EXPORT_SHIT
#include <omega/surface_raylib.h>

OMG_API bool omg_window_raylib_set_size(OMG_WindowRaylib* this, const OMG_FPoint* new_size);
OMG_API bool omg_window_raylib_set_state(OMG_WindowRaylib* this, int state);
OMG_API bool omg_window_raylib_set_sys_button(OMG_WindowRaylib* this, int id, bool enabled);
OMG_API bool omg_window_raylib_set_resizable(OMG_WindowRaylib* this, bool enabled);
OMG_API bool omg_window_raylib_set_bordered(OMG_WindowRaylib* this, bool enabled);
OMG_API bool omg_window_raylib_set_always_on_top(OMG_WindowRaylib* this, bool enabled);
OMG_API bool omg_window_raylib_set_window_mode(OMG_WindowRaylib* this, int mode);
OMG_API bool omg_window_raylib_show(OMG_WindowRaylib* this, bool show);
OMG_API bool omg_window_raylib_set_title(OMG_WindowRaylib* this, const OMG_String* new_title);
OMG_API bool omg_window_raylib_renderer_alloc(OMG_WindowRaylib* this);
OMG_API bool omg_window_raylib_set_icon(OMG_WindowRaylib* this, OMG_SurfaceRaylib* icon);
OMG_API bool omg_window_raylib_mouse_set_shown(OMG_WindowRaylib* this, int show_mode);
OMG_API bool omg_window_raylib_mouse_warp(OMG_WindowRaylib* this, const OMG_FPoint* pos);
OMG_API bool omg_window_raylib_mouse_set_rel(OMG_WindowRaylib* this, int rel_mode);
OMG_API bool omg_window_raylib_mouse_set_system_cursor(OMG_WindowRaylib* this, int cursor_id);
OMG_API int omg_window_raylib_display_get_index(OMG_WindowRaylib* this);
OMG_API bool omg_window_raylib_get_pos(OMG_WindowRaylib* this, OMG_FRect* pos);
OMG_API bool omg_window_raylib_set_pos(OMG_WindowRaylib* this, const OMG_FRect* pos);
OMG_API bool omg_window_raylib_raise(OMG_WindowRaylib* this);
OMG_API bool omg_window_raylib_set_opacity(OMG_WindowRaylib* this, float opacity);
OMG_API float omg_window_raylib_get_opacity(OMG_WindowRaylib* this);
#endif
#endif
