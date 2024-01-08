#include <omega/winmgr.h>
#include <omega/omega.h>
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

OMG_Surface* omg_winmgr_surf_create(OMG_Winmgr* this, const OMG_FPoint* size, bool has_alpha) {
    OMG_UNUSED(this, size, has_alpha);
    return NULL;
}

bool omg_winmgr_surf_destroy(OMG_Winmgr* this, OMG_Surface* surf) {
    OMG_UNUSED(this, surf);
    return false;
}

OMG_Surface* omg_winmgr_surf_from_path(struct OMG_Winmgr* this, const OMG_String* fp) {
    // TODO
    OMG_UNUSED(this, fp);
    return NULL;
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
    this->img->init = omg_image_loader_init;
    return false;
}

bool omg_winmgr_init(OMG_Winmgr* this) {
    this->cache = OMG_MALLOC(omg_base->mem, sizeof(OMG_Window*) * OMG_MAX_WINDOWS);
    if (OMG_ISNULL(this->cache))
        return true;
    omg_base->std->memset(this->cache, 0, sizeof(OMG_Window*) * OMG_MAX_WINDOWS);
    this->surf_depth = 32;
    this->sz_image_loader = sizeof(OMG_ImageLoader);
    this->surf_rle = true;
    this->img = NULL;
    this->destroy = omg_winmgr_destroy;
    this->window_alloc = omg_winmgr_window_alloc;
    this->window_free = omg_winmgr_window_free;
    this->surf_create = omg_winmgr_surf_create;
    this->surf_destroy = omg_winmgr_surf_destroy;
    this->surf_from_path = omg_winmgr_surf_from_path;
    this->image_loader_alloc = omg_winmgr_image_loader_alloc;
    this->inited = true;
    return false;
}
