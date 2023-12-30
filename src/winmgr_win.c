#include <omega/window_win.h>
#include <omega/winmgr_win.h>
#include <omega/omega.h>
#define base ((OMG_Winmgr*)this)
#define omg_base ((OMG_Omega*)base->omg)

#if OMG_SUPPORT_WIN
OMG_WindowWin* omg_winmgr_win_window_alloc(OMG_WinmgrWin* this) {
    OMG_WindowWin* result = OMG_MALLOC(omg_base->mem, sizeof(OMG_WindowWin));
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to allocate memory for Win32 Window");
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_window_fill_on_create(result);
    result->parent.omg = omg_base;
    result->u32 = this->u32;
    result->k32 = this->k32;
    result->dwm = this->dwm;
    result->g32 = this->g32;
    result->win_build_num = this->win_build_num;
    result->uxtheme = this->uxtheme;
    result->parent.was_allocated = true;
    result->parent.default_init = omg_window_win_init;
    OMG_END_POINTER_CAST();
    return result;
}

bool omg_winmgr_win_window_free(OMG_WinmgrWin* this, OMG_WindowWin* window) {
    if (OMG_ISNULL(window))
        return true;
    ((OMG_Window*)window)->destroy((OMG_Window*)window);
    if (((OMG_Window*)window)->was_allocated)
        return OMG_FREE(omg_base->mem, window);
    return false;
}

bool omg_winmgr_win_destroy(OMG_WinmgrWin* this) {
    omg_winmgr_destroy((OMG_Winmgr*)this);
    return false;
}

bool omg_winmgr_win_init(OMG_WinmgrWin* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_winmgr_win_destroy;
    base->window_alloc = omg_winmgr_win_window_alloc;
    base->window_free = omg_winmgr_win_window_free;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
