#include <omega/window_win.h>
#include <omega/winmgr_win.h>
#include <omega/omega.h>

#if OMG_SUPPORT_WIN
#define base ((OMG_Winmgr*)this)
#define surf_base ((OMG_Surface*)surf)
#define omg_base ((OMG_Omega*)base->omg)
#include <omega/surface_win.h>

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
#if OMG_SUPPORT_SDL2
    if (OMG_ISNOTNULL(this->sdl2)) {
        if (this->sdl2->SDL_WasInit(0) > 0)
            this->sdl2->SDL_Quit();
        omg_sdl2_dll_free(this->sdl2);
        OMG_FREE(omg_base->mem, this->sdl2);
        this->sdl2 = NULL;
    }
#endif
    return false;
}

OMG_SurfaceWin* omg_winmgr_win_surf_from_fp(OMG_WinmgrWin* this, OMG_SurfaceWin* surf, const OMG_String* path, int format) {
    if (OMG_ISNULL(surf)) {
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceWin));
        if (OMG_ISNULL(surf))
            return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        surf_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    else
        surf_base->was_allocated = false;
#endif
#if OMG_SUPPORT_OMG_IMAGE
    if (base->img->type == OMG_IMAGE_LOADER_TYPE_OMG) {
        struct {
            void* data;
            int w, h, depth;
        } img_buf;
        if (base->img->image_from_fp_internal(base->img, path, &img_buf, format)) {
            OMG_FREE(omg_base->mem, surf);
            return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
        }
        surf_base->data = img_buf.data;
        surf_base->size.w = (float)img_buf.w;
        surf_base->size.h = (float)img_buf.h;
        surf_base->has_alpha = true;
    }
#endif
    return surf;
}

OMG_SurfaceWin* omg_winmgr_win_surf_create(OMG_WinmgrWin* this, OMG_SurfaceWin* surf, const OMG_FPoint* size, bool has_alpha) {
    if (OMG_ISNULL(surf)) {
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceWin));
        if (OMG_ISNULL(surf))
            return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        surf_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    else
        surf_base->was_allocated = false;
#endif
    surf->dc = this->g32->CreateCompatibleDC(NULL);
    if (OMG_ISNULL(surf->dc)) {
        OMG_FREE(omg_base->mem, surf);
        _OMG_LOG_ERROR(omg_base, "Failed to create Win32 HDC for surface");
        return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
    }
    surf->bm = this->g32->CreateCompatibleBitmap(surf->dc, (int)size->w, (int)size->h);
    if (OMG_ISNULL(surf->bm)) {
        this->g32->DeleteDC(surf->dc);
        OMG_FREE(omg_base->mem, surf);
        _OMG_LOG_ERROR(omg_base, "Failed to create Win32 bitmap for surface");
        return (OMG_SurfaceWin*)omg_winmgr_dummy_surf_create(base);
    }
    surf_base->data = NULL;
    surf_base->size.w = size->w;
    surf_base->size.h = size->h;
    surf_base->has_alpha = has_alpha;
    return surf;
}

bool omg_winmgr_win_surf_destroy(OMG_WinmgrWin* this, OMG_SurfaceWin* surf) {
    if (OMG_ISNULL(surf) || OMG_ISNULL(surf->dc) || OMG_ISNULL(surf->bm)) {
        _OMG_NULL_SURFACE_WARN();
        return true;
    }
    bool res = false;
    if (OMG_ISNULL(surf_base->data)) {
        if (!this->g32->DeleteObject(surf->bm)) {
            res = true;
            _OMG_LOG_WARN(omg_base, "Failed to delete win32 bitmap");
        }
        if (!this->g32->DeleteDC(surf->dc)) {
            res = true;
            _OMG_LOG_WARN(omg_base, "Failed to delete Win32 DC");
        }
    }
    else {
        OMG_FREE(omg_base->mem, surf_base->data);
        surf_base->data = NULL;
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    if (surf_base->was_allocated)
#endif
    OMG_FREE(omg_base->mem, surf);
    return res;
}

bool omg_winmgr_win_init(OMG_WinmgrWin* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    base->fnt_type = OMG_FONT_MGR_SDL2;
#if OMG_SUPPORT_SDL2
    this->sdl2 = NULL;
#endif
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_winmgr_win_destroy;
    base->window_alloc = omg_winmgr_win_window_alloc;
    base->window_free = omg_winmgr_win_window_free;
    base->surf_create = omg_winmgr_win_surf_create;
    base->surf_destroy = omg_winmgr_win_surf_destroy;
    base->surf_from_fp = omg_winmgr_win_surf_from_fp;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
