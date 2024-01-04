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

bool omg_winmgr_init(OMG_Winmgr* this) {
    this->cache = OMG_MALLOC(omg_base->mem, sizeof(OMG_Window*) * OMG_MAX_WINDOWS);
    if (OMG_ISNULL(this->cache))
        return true;
    omg_base->std->memset(this->cache, 0, sizeof(OMG_Window*) * OMG_MAX_WINDOWS);
    this->destroy = omg_winmgr_destroy;
    this->window_alloc = omg_winmgr_window_alloc;
    this->window_free = omg_winmgr_window_free;
    this->surf_create = omg_winmgr_surf_create;
    this->surf_destroy = omg_winmgr_surf_destroy;
    return false;
}
