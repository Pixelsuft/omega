#include <omega/window_win.h>
#include <omega/winmgr_win.h>
#include <omega/omega.h>
#define base ((OMG_Winmgr*)this)
#define omg_base ((OMG_Omega*)base->omg)

#if OMG_SUPPORT_WIN
bool omg_winmgr_win_destroy(OMG_WinmgrWin* this) {
    omg_winmgr_destroy((OMG_Winmgr*)this);
    return false;
}

bool omg_winmgr_win_init(OMG_WinmgrWin* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    return false;
}

OMG_Window* omg_winmgr_win_window_alloc(OMG_WinmgrWin* this) {
    OMG_UNUSED(this);
    return NULL;
}

bool omg_winmgr_win_window_free(OMG_WinmgrWin* this, OMG_Window* window) {
    if (OMG_ISNULL(window))
        return true;
    return OMG_FREE(omg_base->mem, window);
}
#endif
