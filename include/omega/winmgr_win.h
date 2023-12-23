#pragma once
#include <omega/winmgr.h>

#if OMG_SUPPORT_WIN
typedef struct {
    OMG_Winmgr parent;
} OMG_WinmgrWin;

OMG_API bool omg_winmgr_win_destroy(OMG_WinmgrWin* this);
OMG_API bool omg_winmgr_win_init(OMG_WinmgrWin* this);
OMG_API OMG_Window* omg_winmgr_win_window_alloc(OMG_WinmgrWin* this);
OMG_API bool omg_winmgr_win_window_free(OMG_WinmgrWin* this, OMG_Window* window);
#endif
