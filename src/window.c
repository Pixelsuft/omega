#include <omega/omega.h>
#include <omega/window.h>
#define omg_base ((OMG_Omega*)this->omg)

void omg_window_fill_on_create(OMG_Window* this) {
    this->type = OMG_WIN_TYPE_NONE;
    this->was_allocated = false;
    this->size.w = 800.0f;
    this->size.h = 600.0f;
    this->extra1 = this->extra2 = this->extra3 = this->extra4 = this->extra5 = NULL;
}

bool omg_window_show(OMG_Window* this, bool show) {
    OMG_UNUSED(this, show);
    return false;
}

bool omg_window_set_title(OMG_Window* this, const OMG_String* new_title) {
    OMG_UNUSED(this, new_title);
    return false;
}

bool omg_window_init(OMG_Window* this) {
    this->destroy = omg_window_destroy;
    this->show = omg_window_show;
    this->set_title = omg_window_set_title;
    for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) {
        if (OMG_ISNULL(omg_base->winmgr->cache[i])) {
            omg_base->winmgr->cache[i] = this;
            break;
        }
    }
    return false;
}

bool omg_window_destroy(OMG_Window* this) {
    for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) {
        if (omg_base->winmgr->cache[i] == this) {
            omg_base->winmgr->cache[i] = NULL;
        }
    }
    return false;
}
