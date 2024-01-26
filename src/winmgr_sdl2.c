#include <omega/window_sdl2.h>
#include <omega/omega.h>

#if OMG_SUPPORT_SDL2
#include <omega/winmgr_sdl2.h>
#include <omega/surface_sdl2.h>
#include <omega/image_sdl2.h>
#include <omega/image_omg.h>
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

OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_create(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, const OMG_FPoint* size, bool has_alpha) {
    if (OMG_ISNULL(surf))
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceSdl2));
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
    surf->extra1 = NULL;
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
    if (OMG_ISNULL(surf) || OMG_ISNULL(surf_base->data)) {
        _OMG_NULL_SURFACE_WARN();
        return true;
    }
    if (OMG_ISNULL(surf->surf)) {
        _OMG_LOG_WARN(omg_base, "Attempted to free SDL2 null surface");
        return true;
    }
    this->sdl2->SDL_FreeSurface(surf->surf);
    if (OMG_ISNOTNULL(surf->extra1)) {
        OMG_FREE(omg_base->mem, surf->extra1);
        surf->extra1 = NULL;
    }
    surf->surf = NULL;
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    if (surf_base->was_allocated)
#endif
    OMG_FREE(omg_base->mem, surf);
    return false;
}

OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_from_fp(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, const OMG_String* path, int format) {
    if (OMG_ISNULL(surf))
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceSdl2));
    if (OMG_ISNULL(surf))
        return (OMG_SurfaceSdl2*)omg_winmgr_dummy_surf_create(base);
#if OMG_SUPPORT_SDL2_IMAGE
    if (base->img->type == OMG_IMAGE_LOADER_TYPE_SDL2) {
        if (base->img->image_from_internal(base->img, 0, path, (void*)&surf->surf, format)) {
            OMG_FREE(omg_base->mem, surf);
            return (OMG_SurfaceSdl2*)omg_winmgr_dummy_surf_create(base);
        }
        surf->extra1 = NULL;
    }
#endif
#if OMG_SUPPORT_OMG_IMAGE
    if (base->img->type == OMG_IMAGE_LOADER_TYPE_OMG) {
        struct {
            void* data;
            int w, h, depth;
        } img_buf;
        if (base->img->image_from_internal(base->img, 0, path, &img_buf, format)) {
            OMG_FREE(omg_base->mem, surf);
            return (OMG_SurfaceSdl2*)omg_winmgr_dummy_surf_create(base);
        }
        surf->surf = this->sdl2->SDL_CreateRGBSurfaceWithFormatFrom(
            img_buf.data,
            img_buf.w, img_buf.h,
            img_buf.depth, img_buf.w * 4,
            SDL_PIXELFORMAT_ABGR8888
        );
        if (OMG_ISNULL(surf->surf)) {
            _OMG_LOG_WARN(omg_base, "Failed load surface from memory (", this->sdl2->SDL_GetError(), ")");
            OMG_FREE(omg_base->mem, img_buf.data);
            return (OMG_SurfaceSdl2*)omg_winmgr_dummy_surf_create(base);
        }
        surf->extra1 = img_buf.data;
    }
#endif
    surf_base->has_alpha = surf->surf->format->Amask > 0;
    surf_base->size.w = (float)surf->surf->w;
    surf_base->size.h = (float)surf->surf->h;
    surf_base->data = (void*)surf->surf->pixels;
    if (this->sdl2->SDL_SetSurfaceBlendMode(surf->surf, surf_base->has_alpha ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE) < 0)
        _OMG_SURF_BLEND_WARN();
    if (this->sdl2->SDL_SetSurfaceRLE(surf->surf, base->surf_rle ? 1 : 0) < 0)
        _OMG_LOG_WARN(omg_base, "Failed to set surface RLE (", this->sdl2->SDL_GetError(), ")");
    return surf;
}

OMG_SurfaceSdl2* omg_winmgr_sdl2_surf_from_mem(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, const void* mem, size_t size, int format) {
    if (OMG_ISNULL(surf))
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceSdl2));
    if (OMG_ISNULL(surf))
        return (OMG_SurfaceSdl2*)omg_winmgr_dummy_surf_create(base);
    OMG_DataWithSize data = { .data = mem, .size = size };
#if OMG_SUPPORT_SDL2_IMAGE
    if (base->img->type == OMG_IMAGE_LOADER_TYPE_SDL2) {
        if (base->img->image_from_internal(base->img, 1, &data, (void*)&surf->surf, format)) {
            OMG_FREE(omg_base->mem, surf);
            return (OMG_SurfaceSdl2*)omg_winmgr_dummy_surf_create(base);
        }
        surf->extra1 = NULL;
    }
#endif
#if OMG_SUPPORT_OMG_IMAGE
    if (base->img->type == OMG_IMAGE_LOADER_TYPE_OMG) {
        struct {
            void* data;
            int w, h, depth;
        } img_buf;
        if (base->img->image_from_internal(base->img, 1, &data, &img_buf, format)) {
            OMG_FREE(omg_base->mem, surf);
            return (OMG_SurfaceSdl2*)omg_winmgr_dummy_surf_create(base);
        }
        surf->surf = this->sdl2->SDL_CreateRGBSurfaceWithFormatFrom(
            img_buf.data,
            img_buf.w, img_buf.h,
            img_buf.depth, img_buf.w * 4,
            SDL_PIXELFORMAT_ABGR8888
        );
        if (OMG_ISNULL(surf->surf)) {
            _OMG_LOG_WARN(omg_base, "Failed load surface from memory (", this->sdl2->SDL_GetError(), ")");
            OMG_FREE(omg_base->mem, img_buf.data);
            return (OMG_SurfaceSdl2*)omg_winmgr_dummy_surf_create(base);
        }
        surf->extra1 = img_buf.data;
    }
#endif
    surf_base->has_alpha = surf->surf->format->Amask > 0;
    surf_base->size.w = (float)surf->surf->w;
    surf_base->size.h = (float)surf->surf->h;
    surf_base->data = (void*)surf->surf->pixels;
    if (this->sdl2->SDL_SetSurfaceBlendMode(surf->surf, surf_base->has_alpha ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE) < 0)
        _OMG_SURF_BLEND_WARN();
    if (this->sdl2->SDL_SetSurfaceRLE(surf->surf, base->surf_rle ? 1 : 0) < 0)
        _OMG_LOG_WARN(omg_base, "Failed to set surface RLE (", this->sdl2->SDL_GetError(), ")");
    return surf;
}

bool omg_winmgr_sdl2_surf_set_locked(OMG_WinmgrSdl2* this, OMG_SurfaceSdl2* surf, bool locked) {
    if (SDL_MUSTLOCK(surf->surf)) {
        if ((locked ? this->sdl2->SDL_LockSurface : this->sdl2->SDL_UnlockSurface)(surf->surf) < 0) {
            _OMG_LOG_WARN(omg_base, "Failed to set surface locked (", this->sdl2->SDL_GetError(), ")");
            return true;
        }
    }
    return false;
}

bool omg_winmgr_sdl2_init(OMG_WinmgrSdl2* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    base->fnt_type = OMG_FONT_MGR_SDL2;
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_winmgr_sdl2_destroy;
    base->window_alloc = omg_winmgr_sdl2_window_alloc;
    base->window_free = omg_winmgr_sdl2_window_free;
    base->surf_create = omg_winmgr_sdl2_surf_create;
    base->surf_destroy = omg_winmgr_sdl2_surf_destroy;
    base->surf_from_fp = omg_winmgr_sdl2_surf_from_fp;
    base->surf_from_mem = omg_winmgr_sdl2_surf_from_mem;
    base->surf_set_locked = omg_winmgr_sdl2_surf_set_locked;
#if OMG_SUPPORT_SDL2_IMAGE
    base->sz_image_loader = sizeof(OMG_ImageLoaderSdl2);
    base->_img_init_ptr = (void*)((size_t)omg_image_loader_sdl2_init);
#endif
    OMG_END_POINTER_CAST();
    return false;
}
#endif
