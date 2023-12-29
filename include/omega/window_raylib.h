#pragma once
#include <omega/window.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/api_raylib.h>

typedef struct {
    OMG_Window parent;
    OMG_Raylib* raylib;
} OMG_WindowRaylib;

OMG_API bool omg_window_raylib_init(OMG_WindowRaylib* this);
OMG_API bool omg_window_raylib_destroy(OMG_WindowRaylib* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_window_raylib_set_state(OMG_WindowRaylib* this, int state);
OMG_API bool omg_window_raylib_set_sys_button(OMG_WindowRaylib* this, int id, bool enabled);
OMG_API bool omg_window_raylib_set_resizable(OMG_WindowRaylib* this, bool enabled);
OMG_API bool omg_window_raylib_set_bordered(OMG_WindowRaylib* this, bool enabled);
OMG_API bool omg_window_raylib_set_always_on_top(OMG_WindowRaylib* this, bool enabled);
OMG_API bool omg_window_raylib_set_window_mode(OMG_WindowRaylib* this, int mode);
OMG_API bool omg_window_raylib_show(OMG_WindowRaylib* this, bool show);
OMG_API bool omg_window_raylib_set_title(OMG_WindowRaylib* this, const OMG_String* new_title);
OMG_API bool omg_window_raylib_renderer_alloc(OMG_WindowRaylib* this);
#endif
#endif
