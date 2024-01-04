#include <omega/winmgr_raylib.h>
#include <omega/omega.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/window_raylib.h>
#include <omega/surface_raylib.h>
#define base ((OMG_Winmgr*)this)
#define surf_base ((OMG_Surface*)surf)
#define omg_base ((OMG_Omega*)base->omg)

OMG_WindowRaylib* omg_winmgr_raylib_window_alloc(OMG_WinmgrRaylib* this) {
    if (OMG_ISNOTNULL(base->cache[0])) {
        _OMG_LOG_ERROR(omg_base, "Failed to create Raylib window (cannot create more than one)");
        return NULL;
    }
    OMG_WindowRaylib* result = OMG_MALLOC(omg_base->mem, sizeof(OMG_WindowRaylib));
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(omg_base, "Failed to allocate memory for Raylib window");
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

OMG_SurfaceRaylib* omg_winmgr_raylib_surf_create(OMG_WinmgrRaylib* this, const OMG_FPoint* size, bool has_alpha) {
    OMG_SurfaceRaylib* surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceRaylib));
    if (OMG_ISNULL(surf))
        return NULL;
    surf->img = this->raylib->LoadImageFromScreen(); // Fuck
    this->raylib->ImageResize(&surf->img, (int)size->w, (int)size->h);
    this->raylib->ImageFormat(&surf->img, has_alpha ? PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 : PIXELFORMAT_UNCOMPRESSED_R8G8B8);
    if (!this->raylib->IsImageReady(surf->img)) {
        OMG_FREE(omg_base->mem, surf);
        return NULL;
    }
    return surf;
}

bool omg_winmgr_raylib_surf_destroy(OMG_WinmgrRaylib* this, OMG_SurfaceRaylib* surf) {
    if (OMG_ISNULL(surf)) {
        _OMG_NULL_SURFACE_WARN();
        return true;
    }
    if (OMG_ISNULL(surf->img.data) || !this->raylib->IsImageReady(surf->img)) {
        _OMG_LOG_WARN(omg_base, "Attempted to free Raylib null surface");
        return true;
    }
    this->raylib->UnloadImage(surf->img);
    surf->img.data = NULL;
    OMG_FREE(omg_base->mem, surf);
    return false;
}

bool omg_winmgr_raylib_init(OMG_WinmgrRaylib* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_winmgr_raylib_destroy;
    base->window_alloc = omg_winmgr_raylib_window_alloc;
    base->window_free = omg_winmgr_raylib_window_free;
    base->surf_create = omg_winmgr_raylib_surf_create;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
