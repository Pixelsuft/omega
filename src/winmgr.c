#include <omega/winmgr.h>
#include <omega/omega.h>
#include <omega/image_omg.h>
#include <omega/font_raylib.h>
#include <omega/font_sdl2.h>
#define omg_base ((OMG_Omega*)this->omg)

OMG_Window* omg_winmgr_window_alloc(OMG_Winmgr* this) {
    OMG_UNUSED(this);
    return NULL;
}

bool omg_winmgr_window_free(OMG_Winmgr* this, OMG_Window* window) {
    if (OMG_ISNULL(window))
        return true;
    return OMG_FREE(omg_base->mem, window);
}

bool omg_winmgr_image_loader_free(OMG_Winmgr* this) {
    if (OMG_ISNOTNULL(this->img)) {
        if (this->img->inited)
            this->img->destroy(this->img);
        if (this->img->was_allocated) {
            this->img->was_allocated = false;
            OMG_FREE(omg_base->mem, this->img);
            this->img = NULL;
        }
    }
    return false;
}

bool omg_winmgr_destroy(OMG_Winmgr* this) {
    if (!this->inited)
        return false;
    if (OMG_ISNOTNULL(this->cache)) {
        for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) {
            OMG_Window* win = this->cache[i];
            if (OMG_ISNULL(win))
                continue;
            win->destroy(win);
            if (win->was_allocated)
                this->window_free(this, win);
        }
        OMG_FREE(omg_base->mem, this->cache);
        this->cache = NULL;
    }
    this->fontmgr_free(this);
    this->image_loader_free(this);
    return false;
}

OMG_Surface* omg_winmgr_surf_create(OMG_Winmgr* this, OMG_Surface* surf, const OMG_FPoint* size, bool has_alpha) {
    OMG_UNUSED(this, surf, size, has_alpha);
    return NULL;
}

bool omg_winmgr_surf_destroy(OMG_Winmgr* this, OMG_Surface* surf) {
    OMG_UNUSED(this, surf);
    return false;
}

OMG_Surface* omg_winmgr_dummy_surf_create(OMG_Winmgr* this) {
    OMG_UNUSED(this);
#if OMG_ALLOW_DUMMY_SURF
    static OMG_Surface surf;
    surf.data = NULL;
    surf.has_alpha = false;
    surf.size.w = surf.size.h = 0.0f;
    return &surf;
#else
    return NULL;
#endif
}

OMG_Surface* omg_winmgr_surf_from_fp(OMG_Winmgr* this, OMG_Surface* surf, const OMG_String* path, int format) {
    OMG_UNUSED(surf, path, format);
    return omg_winmgr_dummy_surf_create(this);
}

OMG_Surface* omg_winmgr_surf_from_mem(OMG_Winmgr* this, OMG_Surface* surf, const void* mem, size_t size, int format) {
    OMG_UNUSED(this, surf, mem, size, format);
    return omg_winmgr_dummy_surf_create(this);
}

OMG_Surface* omg_winmgr_surf_from_file(OMG_Winmgr* this, OMG_Surface* surf, OMG_File* file, bool destroy_file, int format) {
    if (OMG_ISNULL(file)) {
        _OMG_LOG_WARN(omg_base, "Null pointer passed for creating surf from file");
        return omg_winmgr_dummy_surf_create(this);
    }
    int64_t file_size = file->get_size(file);
    if (file_size <= 0) {
        if (destroy_file)
            file->destroy(file);
        return omg_winmgr_dummy_surf_create(this);
    }
    void* buf = OMG_MALLOC(omg_base->mem, file_size);
    if (OMG_ISNULL(buf)) {
        if (destroy_file)
            file->destroy(file);
        return omg_winmgr_dummy_surf_create(this);
    }
    size_t size_read = file->read(file, buf, 1, (size_t)file_size);
    if (destroy_file)
        file->destroy(file);
    if (size_read == 0) {
        OMG_FREE(omg_base->mem, buf);
        return omg_winmgr_dummy_surf_create(this);
    }
    OMG_Surface* res = this->surf_from_mem(this, surf, buf, size_read, format);
    OMG_FREE(omg_base->mem, buf);
    return res;
}

bool omg_winmgr_surf_set_locked(OMG_Winmgr* this, OMG_Surface* surf, bool locked) {
    OMG_UNUSED(this, surf, locked);
    return false;
}

bool omg_winmgr_image_loader_alloc(OMG_Winmgr* this) {
    if (OMG_ISNULL(this->img)) {
        this->img = OMG_MALLOC(omg_base->mem, this->sz_image_loader);
        if (OMG_ISNULL(this->img))
            return true;
        this->img->was_allocated = true;
    }
    else
        this->img->was_allocated = false;
    this->img->inited = false;
    OMG_BEGIN_POINTER_CAST();
    this->img->init = this->_img_init_ptr;
    OMG_END_POINTER_CAST();
    this->img->omg = omg_base;
    return false;
}

bool omg_winmgr_fontmgr_free(OMG_Winmgr* this) {
    if (OMG_ISNOTNULL(this->fnt)) {
        if (this->fnt->inited)
            this->fnt->destroy(this->fnt);
        if (this->fnt->was_allocated) {
            this->fnt->was_allocated = false;
            OMG_FREE(omg_base->mem, this->fnt);
            this->fnt = NULL;
        }
    }
    return false;
}

bool omg_winmgr_fontmgr_alloc(OMG_Winmgr* this) {
    size_t sz_font_mgr = sizeof(OMG_FontMgr);
#if OMG_SUPPORT_SDL2_TTF
    if (this->fnt_type == OMG_FONT_MGR_SDL2)
        sz_font_mgr = sizeof(OMG_FontMgrSdl2);
#endif
#if OMG_SUPPORT_RAYLIB
    if (this->fnt_type == OMG_FONT_MGR_RAYLIB)
        sz_font_mgr = sizeof(OMG_FontMgrRaylib);
#endif
    if (OMG_ISNULL(this->fnt)) {
        this->fnt = OMG_MALLOC(omg_base->mem, sz_font_mgr);
        if (OMG_ISNULL(this->fnt))
            return true;
        this->fnt->was_allocated = true;
    }
    else
        this->fnt->was_allocated = false;
    this->fnt->inited = false;
    this->fnt->init = omg_fontmgr_init;
    OMG_BEGIN_POINTER_CAST();
#if OMG_SUPPORT_SDL2_TTF
    if (this->fnt_type == OMG_FONT_MGR_SDL2)
        this->fnt->init = omg_fontmgr_sdl2_init;
#endif
#if OMG_SUPPORT_RAYLIB
    if (this->fnt_type == OMG_FONT_MGR_RAYLIB)
        this->fnt->init = omg_fontmgr_raylib_init;
#endif
    OMG_END_POINTER_CAST();
    this->fnt->omg = omg_base;
    return false;
}

int omg_winmgr_display_get_count(OMG_Winmgr* this) {
    OMG_UNUSED(this);
    return -1;
}

const OMG_String* omg_winmgr_display_get_name(OMG_Winmgr* this, int display_id) {
    OMG_UNUSED(this, display_id);
    return &OMG_STRING_MAKE_STATIC("");
}

bool omg_winmgr_display_get_bounds(OMG_Winmgr* this, int display_id, OMG_FRect* rect, bool only_usable) {
    OMG_UNUSED(this, display_id, only_usable);
    rect->x = rect->y = rect->w = rect->h = 0.0f;
    return true;
}

bool omg_winmgr_display_get_scale(OMG_Winmgr* this, int display_id, OMG_FRect* scale) {
    OMG_UNUSED(this, display_id);
    scale->px = scale->py = scale->pz = scale->pw = 1.0f;
    return true;
}

bool omg_winmgr_init(OMG_Winmgr* this) {
    this->cache = OMG_MALLOC(omg_base->mem, sizeof(OMG_Window*) * OMG_MAX_WINDOWS);
    if (OMG_ISNULL(this->cache))
        return true;
    omg_base->std->memset(this->cache, 0, sizeof(OMG_Window*) * OMG_MAX_WINDOWS);
    this->surf_depth = 32;
    this->surf_rle = true;
    this->img = NULL;
    this->fnt = NULL;
    this->image_formats = OMG_IMG_FORMAT_BMP | OMG_IMG_FORMAT_JPG | OMG_IMG_FORMAT_PNG;
    this->fnt_type = OMG_FONT_MGR_NONE;
#if OMG_SUPPORT_OMG_IMAGE
    this->_img_init_ptr = (void*)((size_t)omg_image_loader_omg_init);
    this->sz_image_loader = sizeof(OMG_ImageLoaderOmg);
#else
    this->_img_init_ptr = (void*)((size_t)omg_image_loader_init);
    this->sz_image_loader = sizeof(OMG_ImageLoader);
#endif
    this->destroy = omg_winmgr_destroy;
    this->window_alloc = omg_winmgr_window_alloc;
    this->window_free = omg_winmgr_window_free;
    this->surf_create = omg_winmgr_surf_create;
    this->surf_destroy = omg_winmgr_surf_destroy;
    this->surf_from_fp = omg_winmgr_surf_from_fp;
    this->surf_from_mem = omg_winmgr_surf_from_mem;
    OMG_BEGIN_POINTER_CAST();
    this->surf_from_file = omg_winmgr_surf_from_file;
    OMG_END_POINTER_CAST();
    this->image_loader_alloc = omg_winmgr_image_loader_alloc;
    this->image_loader_free = omg_winmgr_image_loader_free;
    this->fontmgr_alloc = omg_winmgr_fontmgr_alloc;
    this->fontmgr_free = omg_winmgr_fontmgr_free;
    this->surf_set_locked = omg_winmgr_surf_set_locked;
    this->display_get_count = omg_winmgr_display_get_count;
    this->display_get_name = omg_winmgr_display_get_name;
    this->inited = true;
    return false;
}
