#pragma once
#include <omega/api_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/surface.h>

typedef struct {
    OMG_Surface parent;
    SDL_Surface* surf;
} OMG_SurfaceSdl2;

#if OMG_EXPORT_SHIT
OMG_API OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_create(OMG_WinmgrSdl2* this, const OMG_FPoint* size, bool has_alpha);
OMG_API bool omg_winmgr_surf_sdl2_destroy(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf);
#endif
#endif
