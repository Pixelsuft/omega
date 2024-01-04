#pragma once
#include <omega/api_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/surface.h>

typedef struct {
    OMG_Surface parent;
    SDL_Surface* surf;
} OMG_SurfaceSdl2;
#endif
