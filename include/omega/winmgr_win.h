#pragma once
#include <omega/winmgr.h>

#if OMG_SUPPORT_WIN
#include <omega/api_sdl2.h>

typedef struct {
    OMG_Winmgr parent;
    OMG_User32* u32;
    OMG_Gdi32* g32;
    OMG_Kernel32* k32;
    OMG_Dwmapi* dwm;
    OMG_Uxtheme* uxtheme;
#if OMG_SUPPORT_SDL2
    OMG_Sdl2* sdl2;
#endif
    int need_display;
    int mon_counter;
    OMG_FRect temp_rect;
    int win_build_num;
} OMG_WinmgrWin;

OMG_API bool omg_winmgr_win_destroy(OMG_WinmgrWin* this);
OMG_API bool omg_winmgr_win_init(OMG_WinmgrWin* this);
OMG_API OMG_WindowWin* omg_winmgr_win_window_alloc(OMG_WinmgrWin* this);
OMG_API bool omg_winmgr_win_window_free(OMG_WinmgrWin* this, OMG_WindowWin* window);
#if OMG_EXPORT_SHIT
#include <omega/surface.h>
OMG_API OMG_SurfaceWin* omg_winmgr_win_surf_create(OMG_WinmgrWin* this, OMG_Surface* surf, const OMG_FPoint* size, bool has_alpha);
OMG_API bool omg_winmgr_win_surf_destroy(OMG_WinmgrWin* this, OMG_SurfaceWin* surf);
OMG_API OMG_SurfaceWin* omg_winmgr_win_surf_from_mem(OMG_WinmgrWin* this, OMG_SurfaceWin* surf, const void* mem, size_t size, int format);
OMG_API OMG_SurfaceWin* omg_winmgr_win_surf_from_fp(OMG_WinmgrWin* this, OMG_SurfaceWin* surf, const OMG_String* path, int format);
OMG_API int omg_winmgr_win_display_get_count(OMG_WinmgrWin* this);
OMG_API OMG_String omg_winmgr_win_display_get_name(OMG_WinmgrWin* this, int display_id);
OMG_API bool omg_winmgr_win_find_display(OMG_WinmgrWin* this, DISPLAY_DEVICEW* monitor_dev, int display_id);
OMG_API bool omg_winmgr_win_display_get_bounds(OMG_WinmgrWin* this, int display_id, OMG_FRect* rect, bool only_usable);
#endif
#endif
