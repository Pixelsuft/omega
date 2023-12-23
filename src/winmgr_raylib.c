#include <omega/winmgr_raylib.h>
#include <omega/window_raylib.h>
#include <omega/omega.h>
#define base ((OMG_Winmgr*)this)
#define omg_base ((OMG_Omega*)base->omg)

#if OMG_SUPPORT_RAYLIB
OMG_WindowRaylib* omg_winmgr_raylib_window_alloc(OMG_WinmgrRaylib* this) {
    OMG_WindowRaylib* result = OMG_MALLOC(omg_base->mem, sizeof(OMG_WindowRaylib));
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to allocate memory for Raylib Window");
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_window_fill_on_create(result);
    result->raylib = this->raylib;
    result->parent.omg = omg_base;
    result->parent.was_allocated = true;
    result->parent.default_init = omg_window_raylib_init;
    OMG_END_POINTER_CAST();
    return result;
}

bool omg_winmgr_raylib_window_free(OMG_WinmgrRaylib* this, OMG_WindowRaylib* window) {
    if (OMG_ISNULL(window))
        return true;
    ((OMG_Window*)window)->destroy((OMG_Window*)window);
    if (((OMG_Window*)window)->was_allocated)
        return OMG_FREE(omg_base->mem, window);
    return false;
}

bool omg_winmgr_raylib_destroy(OMG_WinmgrRaylib* this) {
    omg_winmgr_destroy((OMG_Winmgr*)this);
    return false;
}

bool omg_winmgr_raylib_init(OMG_WinmgrRaylib* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_winmgr_raylib_destroy;
    base->window_alloc = omg_winmgr_raylib_window_alloc;
    base->window_free = omg_winmgr_raylib_window_free;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
