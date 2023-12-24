#pragma once
#include <omega/api_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/renderer.h>
#define _OMG_SDL2_DRAW_COLOR_WARN() _OMG_LOG_WARN(omg_base, "Failed to set draw color (", this->sdl2->SDL_GetError(), ")")

typedef struct {
    OMG_Renderer parent;
    OMG_Sdl2* sdl2;
    SDL_Window* win;
    SDL_Renderer* ren;
} OMG_RendererSdl2;

OMG_API bool omg_renderer_sdl2_init(OMG_RendererSdl2* this);
OMG_API bool omg_renderer_sdl2_destroy(OMG_RendererSdl2* this);
#if OMG_EXPORT_SHIT
OMG_API void omg_renderer_sdl2_update_scale(OMG_RendererSdl2* this);
OMG_API bool omg_renderer_sdl2_clear(OMG_RendererSdl2* this, const OMG_Color* col);
OMG_API bool omg_renderer_sdl2_flip(OMG_RendererSdl2* this);
#endif
#endif
