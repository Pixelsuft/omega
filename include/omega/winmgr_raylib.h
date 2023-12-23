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
#endif
