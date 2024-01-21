#include <omega/winmgr.h>
#include <omega/omega.h>
#include <omega/image_omg.h>
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

OMG_Surface* omg_winmgr_surf_create(OMG_Winmgr* this, const OMG_FPoint* size, bool has_alpha) {
    OMG_UNUSED(this, size, has_alpha);
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

OMG_Surface* omg_winmgr_surf_from_fp(OMG_Winmgr* this, const OMG_String* path, int format) {
    OMG_UNUSED(path, format);
    return omg_winmgr_dummy_surf_create(this);
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
    if (OMG_ISNULL(this->fnt)) {
        this->fnt = OMG_MALLOC(omg_base->mem, this->sz_font_mgr);
        if (OMG_ISNULL(this->fnt))
            return true;
        this->fnt->was_allocated = true;
    }
    else
        this->fnt->was_allocated = false;
    this->fnt->inited = false;
    OMG_BEGIN_POINTER_CAST();
    this->fnt->init = this->_fnt_init_ptr;
    OMG_END_POINTER_CAST();
    this->fnt->omg = omg_base;
    return false;
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
#if OMG_SUPPORT_OMG_IMAGE
    this->_img_init_ptr = (void*)((size_t)omg_image_loader_omg_init);
    this->sz_image_loader = sizeof(OMG_ImageLoaderOmg);
#else
    this->_img_init_ptr = (void*)((size_t)omg_image_loader_init);
    this->sz_image_loader = sizeof(OMG_ImageLoader);
#endif
    this->sz_font_mgr = sizeof(OMG_FontMgr);
    this->_fnt_init_ptr = (void*)((size_t)omg_fontmgr_init);
    this->destroy = omg_winmgr_destroy;
    this->window_alloc = omg_winmgr_window_alloc;
    this->window_free = omg_winmgr_window_free;
    this->surf_create = omg_winmgr_surf_create;
    this->surf_destroy = omg_winmgr_surf_destroy;
    this->surf_from_fp = omg_winmgr_surf_from_fp;
    this->image_loader_alloc = omg_winmgr_image_loader_alloc;
    this->image_loader_free = omg_winmgr_image_loader_free;
    this->fontmgr_alloc = omg_winmgr_fontmgr_alloc;
    this->fontmgr_free = omg_winmgr_fontmgr_free;
    this->surf_set_locked = omg_winmgr_surf_set_locked;
    this->inited = true;
    return false;
}
