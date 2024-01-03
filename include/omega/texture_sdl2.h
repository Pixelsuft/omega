#pragma once
#include <omega/api_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/texture.h>

typedef struct {
    OMG_Texture parent;
    SDL_Texture* tex;
} OMG_TextureSdl2;
#endif
