#pragma once
#include <omega/api_raylib.h>
#include <omega/window_raylib.h>
#include <omega/winmgr.h>

#if OMG_SUPPORT_RAYLIB
typedef struct {
    OMG_Winmgr parent;
    OMG_Raylib* raylib;
} OMG_WinmgrRaylib;

OMG_API bool omg_winmgr_raylib_destroy(OMG_WinmgrRaylib* this);
OMG_API bool omg_winmgr_raylib_init(OMG_WinmgrRaylib* this);
OMG_API OMG_WindowRaylib* omg_winmgr_raylib_window_alloc(OMG_WinmgrRaylib* this);
OMG_API bool omg_winmgr_raylib_window_free(OMG_WinmgrRaylib* this, OMG_WindowRaylib* window);
#if OMG_EXPORT_SHIT
#include <omega/surface_raylib.h>

OMG_API OMG_SurfaceRaylib* omg_winmgr_raylib_surf_from_path(OMG_WinmgrRaylib* this, const OMG_String* path, int format);
OMG_API OMG_SurfaceRaylib* omg_winmgr_raylib_surf_create(OMG_WinmgrRaylib* this, const OMG_FPoint* size, bool has_alpha);
OMG_API bool omg_winmgr_raylib_surf_destroy(OMG_WinmgrRaylib* this, OMG_SurfaceRaylib* surf);
#endif
#endif
