#include <omega/window_sdl2.h>
#include <omega/omega.h>

#if OMG_SUPPORT_SDL2
#include <omega/winmgr_sdl2.h>
#include <omega/surface_sdl2.h>
#define base ((OMG_Winmgr*)this)
#define omg_base ((OMG_Omega*)base->omg)

OMG_WindowSdl2* omg_winmgr_sdl2_window_alloc(OMG_WinmgrSdl2* this) {
    OMG_WindowSdl2* result = OMG_MALLOC(omg_base->mem, sizeof(OMG_WindowSdl2));
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to allocate memory for SDL2 Window");
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_window_fill_on_create(result);
    result->sdl2 = this->sdl2;
    result->parent.omg = omg_base;
    result->parent.was_allocated = true;
    result->parent.default_init = omg_window_sdl2_init;
    OMG_END_POINTER_CAST();
    return result;
}

bool omg_winmgr_sdl2_window_free(OMG_WinmgrSdl2* this, OMG_WindowSdl2* window) {
    if (OMG_ISNULL(window))
        return true;
    ((OMG_Window*)window)->destroy((OMG_Window*)window);
    if (((OMG_Window*)window)->was_allocated)
        return OMG_FREE(omg_base->mem, window);
    return false;
}

bool omg_winmgr_sdl2_destroy(OMG_WinmgrSdl2* this) {
    omg_winmgr_destroy((OMG_Winmgr*)this);
    return false;
}

OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_create(OMG_WinmgrSdl2* this, const OMG_FPoint* size, bool has_alpha) {

}

bool omg_winmgr_sdl2_surf_destroy(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf) {

}

bool omg_winmgr_sdl2_init(OMG_WinmgrSdl2* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_winmgr_sdl2_destroy;
    base->window_alloc = omg_winmgr_sdl2_window_alloc;
    base->window_free = omg_winmgr_sdl2_window_free;
    base->surf_create = omg_winmgr_sdl2_surf_create;
    base->surf_destroy = omg_winmgr_sdl2_surf_destroy;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
