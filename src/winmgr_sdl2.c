#include <omega/window_sdl2.h>
#include <omega/omega.h>

#if OMG_SUPPORT_SDL2
#include <omega/winmgr_sdl2.h>
#include <omega/surface_sdl2.h>
#include <omega/image_sdl2.h>
#define base ((OMG_Winmgr*)this)
#define surf_base ((OMG_Surface*)surf)
#define omg_base ((OMG_Omega*)base->omg)

#define _OMG_SURF_BLEND_WARN() _OMG_LOG_WARN(omg_base, "Failed to set surface blend mode (", this->sdl2->SDL_GetError(), ")")

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
    OMG_SurfaceSdl2* surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceSdl2));
    if (OMG_ISNULL(surf))
        return NULL;
    surf->surf = this->sdl2->SDL_CreateRGBSurfaceWithFormat(
        0,
        (int)size->w, (int)size->h,
        base->surf_depth,
        has_alpha ? SDL_PIXELFORMAT_RGBA8888 : SDL_PIXELFORMAT_RGB888
    );
    if (OMG_ISNULL(surf->surf)) {
        OMG_FREE(omg_base->mem, surf);
        _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 surface (", this->sdl2->SDL_GetError(), ")");
        return NULL;
    }
    surf_base->has_alpha = has_alpha;
    surf_base->size.w = (float)surf->surf->w;
    surf_base->size.h = (float)surf->surf->h;
    surf_base->data = (void*)surf->surf->pixels;
    if (this->sdl2->SDL_SetSurfaceBlendMode(surf->surf, has_alpha ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE) < 0)
        _OMG_SURF_BLEND_WARN();
    if (this->sdl2->SDL_SetSurfaceRLE(surf->surf, base->surf_rle ? 1 : 0) < 0)
        _OMG_LOG_WARN(omg_base, "Failed to set surface RLE (", this->sdl2->SDL_GetError(), ")");
    return surf;
}

bool omg_winmgr_sdl2_surf_destroy(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf) {
    if (OMG_ISNULL(surf)) {
        _OMG_NULL_SURFACE_WARN();
        return true;
    }
    if (OMG_ISNULL(surf->surf)) {
        _OMG_LOG_WARN(omg_base, "Attempted to free SDL2 null surface");
        return true;
    }
    this->sdl2->SDL_FreeSurface(surf->surf);
    surf->surf = NULL;
    OMG_FREE(omg_base->mem, surf);
    return false;
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
#if OMG_SUPPORT_SDL2_IMAGE
    base->sz_image_loader = sizeof(OMG_ImageLoaderSdl2);
    base->_img_init_ptr = (void*)omg_image_loader_sdl2_init;
#endif
    OMG_END_POINTER_CAST();
    return false;
}
#endif
