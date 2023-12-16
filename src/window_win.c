#include <omega/window_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)this->omg)

bool omg_window_win_show(OMG_WindowWin* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_window_win_init(OMG_WindowWin* this) {
    omg_window_init(base);
    base->inited = false;
    base->inited = true;
    OMG_BEGIN_POINTER_CAST();
    base->show = omg_window_win_show;
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_window_win_destroy(OMG_WindowWin* this) {
    OMG_UNUSED(this);
    if (base->inited) {
        base->inited = false;
    }
    return false;
}
#endif
