#include <omega/omega.h>
#include <omega/window.h>
#define omg_base ((OMG_Omega*)this->omg)

void omg_window_fill_on_create(OMG_Window* this) {
    this->type = OMG_WIN_TYPE_NONE;
    this->was_allocated = false;
    this->vsync = true;
    this->resizable = false;
    this->thick = false;
    this->has_border = true;
    this->centered = true;
    this->minimized = this->maximized = false;
    this->always_on_top = false;
    this->window_mode = OMG_WIN_MODE_WINDOW;
    this->sys_buttons[OMG_WIN_SYS_BUTTON_CLOSE] = true;
    this->sys_buttons[OMG_WIN_SYS_BUTTON_MAXIMIZE] = true;
    this->sys_buttons[OMG_WIN_SYS_BUTTON_MINIMIZE] = true;
    this->array_pos = 0;
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

bool omg_window_renderer_alloc(OMG_Window* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_window_renderer_free(OMG_Window* this) {
    if (OMG_ISNOTNULL(this->ren)) {
        // ren->destroy
        if (this->ren->was_allocated) {
            OMG_FREE(omg_base->mem, this->ren);
            this->ren = NULL;
        }
    }
    return false;
}

bool omg_window_destroy(OMG_Window* this) {
    if (OMG_ISNULL(this))
        return true;
    bool result = this->renderer_free(this);
    if (OMG_ISNULL(omg_base->winmgr))
        return true;
    if (omg_base->winmgr->cache[this->array_pos] == this) {
        omg_base->winmgr->cache[this->array_pos] = NULL;
    }
    return result;
}

bool omg_window_init(OMG_Window* this) {
    this->scale.x = this->scale.y = 1.0f;
    this->ren = NULL;
    this->ren_type = OMG_REN_TYPE_AUTO;
    this->destroy = omg_window_destroy;
    this->show = omg_window_show;
    this->set_title = omg_window_set_title;
    this->renderer_alloc = omg_window_renderer_alloc;
    this->renderer_free = omg_window_renderer_free;
    for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) {
        if (OMG_ISNULL(omg_base->winmgr->cache[i])) {
            omg_base->winmgr->cache[i] = this;
            this->array_pos = i;
            break;
        }
    }
    return false;
}
