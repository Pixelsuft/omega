#pragma once
#include <omega/api_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/renderer.h>

typedef struct {
    OMG_Renderer parent;
    OMG_Sdl2* sdl2;
    SDL_Window* win;
    SDL_Renderer* ren;
} OMG_RendererSdl2;

OMG_API bool omg_renderer_sdl2_init(OMG_RendererSdl2* this);
OMG_API bool omg_renderer_sdl2_destroy(OMG_RendererSdl2* this);
#endif
