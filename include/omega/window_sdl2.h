#pragma once
#include <omega/config.h>
#include <omega/window.h>

#if OMG_SUPPORT_SDL2
#include <omega/api_sdl2.h>

typedef struct {
    OMG_Window parent;
    SDL_Window* win;
    OMG_Sdl2* sdl2;
    uint32_t id;
} OMG_WindowSdl2;

OMG_API bool omg_window_sdl2_init(OMG_WindowSdl2* this);
OMG_API bool omg_window_sdl2_destroy(OMG_WindowSdl2* this);
#if OMG_EXPORT_SHIT
#include <omega/surface_sdl2.h>

OMG_API bool omg_window_sdl2_set_size(OMG_WindowSdl2* this, const OMG_FPoint* new_size);
OMG_API bool omg_window_sdl2_set_min_size(OMG_WindowSdl2* this, const OMG_FPoint* new_min_size);
OMG_API bool omg_window_sdl2_set_max_size(OMG_WindowSdl2* this, const OMG_FPoint* new_max_size);
OMG_API bool omg_window_sdl2_set_state(OMG_WindowSdl2* this, int state);
OMG_API bool omg_window_sdl2_set_window_mode(OMG_WindowSdl2* this, int mode);
OMG_API bool omg_window_sdl2_set_sys_button(OMG_WindowSdl2* this, int id, bool enabled);
OMG_API bool omg_window_sdl2_set_resizable(OMG_WindowSdl2* this, bool enabled);
OMG_API bool omg_window_sdl2_set_bordered(OMG_WindowSdl2* this, bool enabled);
OMG_API bool omg_window_sdl2_set_always_on_top(OMG_WindowSdl2* this, bool enabled);
OMG_API bool omg_window_sdl2_show(OMG_WindowSdl2* this, bool show);
OMG_API bool omg_window_sdl2_set_title(OMG_WindowSdl2* this, const OMG_String* new_title);
OMG_API bool omg_window_sdl2_renderer_alloc(OMG_WindowSdl2* this);
OMG_API bool omg_window_sdl2_set_icon(OMG_WindowSdl2* this, OMG_SurfaceSdl2* icon);;
#endif
#endif
