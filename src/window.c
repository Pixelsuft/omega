#include <omega/omega.h>
#include <omega/window.h>
#define omg_base ((OMG_Omega*)this->omg)

void omg_window_fill_on_create(OMG_Window* this) {
    this->type = OMG_WIN_TYPE_NONE;
    this->was_allocated = false;
    this->vsync = true;
    this->resizable = false;
    this->thick = false;
    this->bordered = true;
    this->centered = true;
    this->aa = true;
    this->state = OMG_WIN_STATE_RESTORED;
    this->always_on_top = false;
    this->window_mode = OMG_WIN_MODE_WINDOW;
    this->sys_buttons = -1;
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

bool omg_window_set_state(OMG_Window* this, int state) {
    this->state = state;
    return false;
}

bool omg_window_set_sys_button(OMG_Window* this, int id, bool enabled) {
    if (enabled)
        this->sys_buttons |= id;
    else
        this->sys_buttons &= ~id;
    return false;
}

bool omg_window_set_resizable(OMG_Window* this, bool enabled) {
    this->resizable = enabled;
    return false;
}

bool omg_window_set_bordered(OMG_Window* this, bool enabled) {
    this->bordered = enabled;
    return false;
}

bool omg_window_set_thick(OMG_Window* this, bool enabled) {
    this->thick = enabled;
    return false;
}

bool omg_window_set_always_on_top(OMG_Window* this, bool enabled) {
    this->always_on_top = enabled;
    return false;
}

bool omg_window_set_window_mode(OMG_Window* this, int mode) {
    this->window_mode = mode;
    return false;
}

bool omg_window_init(OMG_Window* this) {
    if (this->sys_buttons < 0) {
        this->sys_buttons = OMG_WIN_SYS_BUTTON_CLOSE | OMG_WIN_SYS_BUTTON_MINIMIZE | (this->resizable ? OMG_WIN_SYS_BUTTON_MAXIMIZE : 0);
    }
    this->scale.x = this->scale.y = 1.0f;
    this->ren = NULL;
    this->allow_alt = true;
    this->allow_alt_f4 = true;
    this->enable_paint = true;
    this->ren_type = OMG_REN_TYPE_AUTO;
    this->destroy = omg_window_destroy;
    this->set_state = omg_window_set_state;
    this->set_sys_button = omg_window_set_sys_button;
    this->set_resizable = omg_window_set_resizable;
    this->set_bordered = omg_window_set_bordered;
    this->set_thick = omg_window_set_thick;
    this->set_always_on_top = omg_window_set_always_on_top;
    this->show = omg_window_show;
    this->set_title = omg_window_set_title;
    this->renderer_alloc = omg_window_renderer_alloc;
    this->renderer_free = omg_window_renderer_free;
    this->set_window_mode = omg_window_set_window_mode;
    for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) {
        if (OMG_ISNULL(omg_base->winmgr->cache[i])) {
            omg_base->winmgr->cache[i] = this;
            this->array_pos = i;
            break;
        }
    }
    return false;
}
