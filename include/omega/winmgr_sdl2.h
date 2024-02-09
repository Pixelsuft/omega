#pragma once
#include <omega/winmgr.h>

#if OMG_SUPPORT_SDL2
typedef struct {
    OMG_Winmgr parent;
    OMG_Sdl2* sdl2;
    SDL_Cursor* cursor_cache;
} OMG_WinmgrSdl2;

OMG_API bool omg_winmgr_sdl2_destroy(OMG_WinmgrSdl2* this);
OMG_API bool omg_winmgr_sdl2_init(OMG_WinmgrSdl2* this);
OMG_API OMG_WindowSdl2* omg_winmgr_sdl2_window_alloc(OMG_WinmgrSdl2* this);
OMG_API bool omg_winmgr_sdl2_window_free(OMG_WinmgrSdl2* this, OMG_WindowSdl2* window);
#if OMG_EXPORT_SHIT
#include <omega/surface_sdl2.h>

OMG_API OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_create(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, const OMG_FPoint* size, bool has_alpha);
OMG_API bool omg_winmgr_surf_sdl2_destroy(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf);
#if OMG_SUPPORT_SDL2_IMAGE
OMG_API OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_from_mem(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, const void* mem, size_t size, int format);
OMG_API bool omg_winmgr_sdl2_surf_set_locked(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, bool locked);
OMG_API OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_from_fp(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, const OMG_String* path, int format);
OMG_API int omg_winmgr_sdl2_display_get_count(OMG_WinmgrSdl2* this);
OMG_API OMG_String omg_winmgr_sdl2_display_get_name(OMG_WinmgrSdl2* this, int display_id);
OMG_API bool omg_winmgr_sdl2_display_get_bounds(OMG_WinmgrSdl2* this, int display_id, OMG_FRect* rect, bool only_usable);
OMG_API bool omg_winmgr_sdl2_display_get_scale(OMG_WinmgrSdl2* this, int display_id, OMG_FRect* dpi);
OMG_API int omg_winmgr_sdl2_display_get_num_modes(OMG_WinmgrSdl2* this, int display_id);
OMG_API bool omg_winmgr_sdl2_display_get_mode(OMG_WinmgrSdl2* this, int display_id, int mode_id, OMG_VideoMode* mode);
OMG_API bool omg_winmgr_sdl2_display_get_current_mode(OMG_WinmgrSdl2* this, int display_id, OMG_VideoMode* mode);
OMG_API bool omg_winmgr_sdl2_display_get_desktop_mode(OMG_WinmgrSdl2* this, int display_id, OMG_VideoMode* mode);
OMG_API int omg_winmgr_sdl2_display_get_orientation(OMG_WinmgrSdl2* this, int display_id);
#endif
#endif
#endif
