#pragma once
#include <omega/winmgr.h>

#if OMG_SUPPORT_WIN
typedef struct {
    OMG_Winmgr parent;
    OMG_User32* u32;
    OMG_Kernel32* k32;
    OMG_Dwmapi* dwm;
    OMG_Uxtheme* uxtheme;
} OMG_WinmgrWin;

OMG_API bool omg_winmgr_win_destroy(OMG_WinmgrWin* this);
OMG_API bool omg_winmgr_win_init(OMG_WinmgrWin* this);
OMG_API OMG_WindowWin* omg_winmgr_win_window_alloc(OMG_WinmgrWin* this);
OMG_API bool omg_winmgr_win_window_free(OMG_WinmgrWin* this, OMG_WindowWin* window);
#endif
