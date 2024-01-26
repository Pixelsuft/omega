#pragma once
#include <omega/winmgr.h>

#if OMG_SUPPORT_SDL2
typedef struct {
    OMG_Winmgr parent;
    OMG_Sdl2* sdl2;
} OMG_WinmgrSdl2;

OMG_API bool omg_winmgr_sdl2_destroy(OMG_WinmgrSdl2* this);
OMG_API bool omg_winmgr_sdl2_init(OMG_WinmgrSdl2* this);
OMG_API OMG_WindowSdl2* omg_winmgr_sdl2_window_alloc(OMG_WinmgrSdl2* this);
OMG_API bool omg_winmgr_sdl2_window_free(OMG_WinmgrSdl2* this, OMG_WindowSdl2* window);
#if OMG_EXPORT_SHIT
#include <omega/surface_sdl2.h>

OMG_API OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_create(OMG_WinmgrSdl2* this, OMG_Surface* surf, const OMG_FPoint* size, bool has_alpha);
OMG_API bool omg_winmgr_surf_sdl2_destroy(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf);
#if OMG_SUPPORT_SDL2_IMAGE
OMG_API OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_from_mem(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, const void* mem, size_t size, int format);
OMG_API bool omg_winmgr_sdl2_surf_set_locked(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, bool locked);
OMG_API OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_from_fp(OMG_WinmgrSdl2* this, OMG_Surface* surf, const OMG_String* path, int format);
#endif
#endif
#endif
