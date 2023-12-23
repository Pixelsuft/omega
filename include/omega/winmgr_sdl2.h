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
#endif
