#include <omega/winmgr_raylib.h>
#include <omega/omega.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/window_raylib.h>
#include <omega/surface_raylib.h>
#include <omega/image_raylib.h>
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

OMG_SurfaceRaylib* omg_winmgr_raylib_surf_create(OMG_WinmgrRaylib* this, OMG_SurfaceRaylib* surf, const OMG_FPoint* size, bool has_alpha) {
    if (OMG_ISNULL(surf)) {
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceRaylib));
        if (OMG_ISNULL(surf))
            return (OMG_SurfaceRaylib*)omg_winmgr_dummy_surf_create(base);
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        surf_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    else
        surf_base->was_allocated = false;
#endif
    surf->img = this->raylib->LoadImageFromScreen(); // Fuck
    if (!this->raylib->IsImageReady(surf->img)) {
        OMG_FREE(omg_base->mem, surf);
        _OMG_LOG_ERROR(omg_base, "Failed to create surface");
        return (OMG_SurfaceRaylib*)omg_winmgr_dummy_surf_create(base);
    }
    this->raylib->ImageResize(&surf->img, (int)size->w, (int)size->h);
    this->raylib->ImageFormat(&surf->img, has_alpha ? PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 : PIXELFORMAT_UNCOMPRESSED_R8G8B8);
    Color col = { .r = 0, .g = 0, .b = 0, .a = 0 };
    this->raylib->ImageClearBackground(&surf->img, col);
    surf_base->has_alpha = has_alpha;
    surf_base->size.w = (float)surf->img.width;
    surf_base->size.h = (float)surf->img.height;
    surf_base->data = surf->img.data;
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
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    if (surf_base->was_allocated)
#endif
    OMG_FREE(omg_base->mem, surf);
    return false;
}

OMG_SurfaceRaylib* omg_winmgr_raylib_surf_from_fp(OMG_WinmgrRaylib* this, OMG_SurfaceRaylib* surf, const OMG_String* path, int format) {
    if (OMG_ISNULL(surf)){
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceRaylib));
        if (OMG_ISNULL(surf))
            return NULL;
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        surf_base->was_allocated = true;
#endif
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    else
        surf_base->was_allocated = false;
#endif
    if (base->img->image_from_internal(base->img, 0, path, &surf->img, format)) {
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        if (surf_base->was_allocated)
#endif
        OMG_FREE(omg_base->mem, surf);
        return NULL;
    }
    surf_base->has_alpha = true; // TODO
    surf_base->size.w = (float)surf->img.width;
    surf_base->size.h = (float)surf->img.height;
    surf_base->data = surf->img.data;
    return surf;
}

OMG_SurfaceRaylib* omg_winmgr_raylib_surf_from_mem(OMG_WinmgrRaylib* this, OMG_SurfaceRaylib* surf, const void* mem, size_t size, int format) {
    if (OMG_ISNULL(surf)){
        surf = OMG_MALLOC(omg_base->mem, sizeof(OMG_SurfaceRaylib));
        if (OMG_ISNULL(surf))
            return NULL;
#if OMG_ALLOW_SURF_WAS_ALLOCATED
        surf_base->was_allocated = false;
#endif
    }
#if OMG_ALLOW_SURF_WAS_ALLOCATED
    else
        surf_base->was_allocated = false;
#endif
    OMG_DataWithSize data = { .data = mem, .size = size };
    if (base->img->image_from_internal(base->img, 1, &data, &surf->img, format)) {
        OMG_FREE(omg_base->mem, surf);
        return NULL;
    }
    surf_base->has_alpha = true; // TODO
    surf_base->size.w = (float)surf->img.width;
    surf_base->size.h = (float)surf->img.height;
    surf_base->data = surf->img.data;
    return surf;
}

int omg_winmgr_raylib_display_get_count(OMG_WinmgrRaylib* this) {
    return this->raylib->GetMonitorCount();
}

OMG_String omg_winmgr_raylib_display_get_name(OMG_WinmgrRaylib* this, int display_id) {
    return OMG_STRING_MAKE_STATIC(this->raylib->GetMonitorName(display_id));
}

bool omg_winmgr_raylib_display_get_bounds(OMG_WinmgrRaylib* this, int display_id, OMG_FRect* rect, bool only_usable) {
    OMG_UNUSED(only_usable);
    Vector2 m_pos = this->raylib->GetMonitorPosition(display_id);
    rect->x = m_pos.x;
    rect->y = m_pos.y;
    rect->w = (float)this->raylib->GetMonitorWidth(display_id);
    rect->h = (float)this->raylib->GetMonitorHeight(display_id);
    return false;
}

bool omg_winmgr_raylib_display_get_scale(OMG_WinmgrRaylib* this, int display_id, OMG_FRect* scale) {
    scale->px = 96.0f * (float)this->raylib->GetMonitorPhysicalWidth(display_id) / (float)this->raylib->GetMonitorWidth(display_id) / 21.5f;
    scale->py = 96.0f * (float)this->raylib->GetMonitorPhysicalHeight(display_id) / (float)this->raylib->GetMonitorHeight(display_id) / 25.77777777777778f;
    scale->pz = (scale->px + scale->py) / 2.0f;
    scale->pw = 1.0f;
    return false;
}

bool omg_winmgr_raylib_display_get_current_mode(OMG_WinmgrRaylib* this, int display_id, OMG_VideoMode* mode) {
    mode->rate = (float)this->raylib->GetMonitorRefreshRate(display_id);
    mode->size.w = (float)this->raylib->GetMonitorWidth(display_id);
    mode->size.h = (float)this->raylib->GetMonitorHeight(display_id);
    return false;
}

bool omg_winmgr_raylib_init(OMG_WinmgrRaylib* this) {
    if (omg_winmgr_init((OMG_Winmgr*)this))
        return true;
    base->fnt_type = OMG_FONT_MGR_RAYLIB;
    base->sz_image_loader = sizeof(OMG_ImageLoaderRaylib);
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_winmgr_raylib_destroy;
    base->display_get_count = omg_winmgr_raylib_display_get_count;
    base->display_get_name = omg_winmgr_raylib_display_get_name;
    base->display_get_bounds = omg_winmgr_raylib_display_get_bounds;
    base->display_get_scale = omg_winmgr_raylib_display_get_scale;
    base->display_get_current_mode = omg_winmgr_raylib_display_get_current_mode;
    base->window_alloc = omg_winmgr_raylib_window_alloc;
    base->window_free = omg_winmgr_raylib_window_free;
    base->surf_create = omg_winmgr_raylib_surf_create;
    base->surf_destroy = omg_winmgr_raylib_surf_destroy;
    base->surf_from_fp = omg_winmgr_raylib_surf_from_fp;
    base->surf_from_mem = omg_winmgr_raylib_surf_from_mem;
    base->_img_init_ptr = (void*)((size_t)omg_image_loader_raylib_init);
    OMG_END_POINTER_CAST();
    return false;
}
#endif
