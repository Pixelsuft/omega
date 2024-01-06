#pragma once
#include <omega/api_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/filesystem.h>

typedef struct {
    OMG_File parent;
    SDL_RWops* rw;
} OMG_FileSdl2;

#endif
