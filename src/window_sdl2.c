#include <omega/window_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/omega.h>
#include <omega/renderer_sdl2.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define ren_sdl2 ((OMG_RendererSdl2*)base->ren)

bool omg_window_sdl2_show(OMG_WindowSdl2* this, bool show) {
    (show ? this->sdl2->SDL_ShowWindow : this->sdl2->SDL_HideWindow)(this->win);
    return false;
}

bool omg_window_sdl2_set_title(OMG_WindowSdl2* this, const OMG_String* new_title) {
    if (omg_string_ensure_null((OMG_String*)new_title))
        return true;
    this->sdl2->SDL_SetWindowTitle(this->win, new_title->ptr);
    return false;
}

bool omg_window_sdl2_renderer_alloc(OMG_WindowSdl2* this) {
    if (base->ren_type != OMG_REN_TYPE_SDL2)
        base->ren_type = OMG_REN_TYPE_AUTO;
    if (base->ren_type == OMG_REN_TYPE_AUTO) {
        base->ren_type = OMG_REN_TYPE_SDL2;
        bool res = omg_window_sdl2_renderer_alloc(this);
        if (res) {
            base->ren_type = OMG_REN_TYPE_AUTO;
        }
        return res;
    }
    if (base->ren_type == OMG_REN_TYPE_SDL2) {
        base->ren = OMG_MALLOC(omg_base->mem, sizeof(OMG_RendererSdl2));
        if (OMG_ISNULL(base->ren))
            return true;
        omg_renderer_fill_on_create(base->ren);
        ren_sdl2->id_cache[0] = -1;
        base->ren->was_allocated = true;
        base->ren->win = this;
        OMG_BEGIN_POINTER_CAST();
        base->ren->init = omg_renderer_sdl2_init;
        base->ren->get_supported_drivers = omg_renderer_sdl2_get_supported_drivers;
        OMG_END_POINTER_CAST();
        ren_sdl2->win = this->win;
        ren_sdl2->sdl2 = this->sdl2;
        return false;
    }
    return true;
}

bool omg_window_sdl2_set_state(OMG_WindowSdl2* this, int state) {
    if (state & OMG_WIN_STATE_MINIMIZED) {
        state &= (~OMG_WIN_STATE_MAXIMIZED) | (~OMG_WIN_STATE_RESTORED);
        this->sdl2->SDL_MinimizeWindow(this->win);
    }
    else if (state & OMG_WIN_STATE_MAXIMIZED) {
        state &= (~OMG_WIN_STATE_MINIMIZED) | (~OMG_WIN_STATE_RESTORED);
        this->sdl2->SDL_MaximizeWindow(this->win);
    }
    else if (/*state & OMG_WIN_STATE_RESTORED*/ !(base->state & OMG_WIN_STATE_RESTORED)) {
        state = OMG_WIN_STATE_RESTORED;
        this->sdl2->SDL_RestoreWindow(this->win);
    }
    base->state = state;
    return false;
}

bool omg_window_sdl2_set_sys_button(OMG_WindowSdl2* this, int id, bool enabled) {
    OMG_UNUSED(this, id, enabled);
    // base->sys_buttons[id] = enabled;
    return false;
}

bool omg_window_sdl2_set_resizable(OMG_WindowSdl2* this, bool enabled) {
    base->resizable = enabled;
    if (base->resizable)
        base->sys_buttons |= OMG_WIN_SYS_BUTTON_MAXIMIZE;
    else
        base->sys_buttons &= ~OMG_WIN_SYS_BUTTON_MAXIMIZE;
    if (OMG_ISNOTNULL(this->sdl2->SDL_SetWindowResizable))
        this->sdl2->SDL_SetWindowResizable(this->win, enabled ? SDL_TRUE : SDL_FALSE);
    return false;
}

bool omg_window_sdl2_set_bordered(OMG_WindowSdl2* this, bool enabled) {
    base->bordered = enabled;
    this->sdl2->SDL_SetWindowBordered(this->win, enabled ? SDL_TRUE : SDL_FALSE);
    return false;
}

bool omg_window_sdl2_set_always_on_top(OMG_WindowSdl2* this, bool enabled) {
    base->always_on_top = enabled;
    if (OMG_ISNOTNULL(this->sdl2->SDL_SetWindowAlwaysOnTop))
        this->sdl2->SDL_SetWindowAlwaysOnTop(this->win, enabled ? SDL_TRUE : SDL_FALSE);
    return false;
}

bool omg_window_sdl2_destroy(OMG_WindowSdl2* this) {
    if (base->inited) {
        omg_window_destroy((OMG_Window*)this);
        this->sdl2->SDL_DestroyWindow(this->win);
        base->inited = false;
    }
    return false;
}

bool omg_window_sdl2_set_window_mode(OMG_WindowSdl2* this, int mode) {
    if (this->sdl2->SDL_SetWindowFullscreen(this->win, (mode == OMG_WIN_MODE_DESKTOP_FULLSCREEN ? SDL_WINDOW_FULLSCREEN_DESKTOP : (
        (mode == OMG_WIN_MODE_FULLSCREEN) ? SDL_WINDOW_FULLSCREEN : 0
    ))) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set window mode (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    return false;
}

bool omg_window_sdl2_init(OMG_WindowSdl2* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_SDL2;
    base->inited = false;
    if (!base->resizable)
        base->sys_buttons &= ~OMG_WIN_SYS_BUTTON_MAXIMIZE;
    this->win = this->sdl2->SDL_CreateWindow(
        "OMG Window [SDL2]",
        base->centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED,
        base->centered ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED,
        (int)base->size.w, (int)base->size.h,
        SDL_WINDOW_HIDDEN |
        (omg_base->support_highdpi ? SDL_WINDOW_ALLOW_HIGHDPI : 0) |
        (base->bordered ? 0 : SDL_WINDOW_BORDERLESS) |
        (base->resizable ? SDL_WINDOW_RESIZABLE : 0) |
        ((base->state & OMG_WIN_STATE_MINIMIZED) ? SDL_WINDOW_MINIMIZED : 0) |
        ((base->state & OMG_WIN_STATE_MAXIMIZED) ? SDL_WINDOW_MAXIMIZED : 0) |
        (base->always_on_top ? SDL_WINDOW_ALWAYS_ON_TOP : 0)
    );
    if (OMG_ISNULL(this->win)) {
        _OMG_LOG_ERROR(omg_base, "Failed to create SDL2 Window (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    this->id = this->sdl2->SDL_GetWindowID(this->win);
    if (this->id == 0) {
        _OMG_LOG_ERROR(omg_base, "Failed to get window id (", this->sdl2->SDL_GetError(), ")");
        this->id = 1;
    }
    OMG_BEGIN_POINTER_CAST();
    base->set_state = omg_window_sdl2_set_title;
    base->set_sys_button = omg_window_sdl2_set_title;
    base->set_resizable = omg_window_sdl2_set_resizable;
    base->set_bordered = omg_window_sdl2_set_bordered;
    base->set_always_on_top = omg_window_sdl2_set_always_on_top;
    base->set_window_mode = omg_window_sdl2_set_window_mode;
    base->renderer_alloc = omg_window_sdl2_renderer_alloc;
    base->show = omg_window_sdl2_show;
    base->set_title = omg_window_sdl2_set_title;
    base->destroy = omg_window_sdl2_destroy;
    OMG_END_POINTER_CAST();
    base->inited = true;
    _OMG_LOG_INFO(omg_base, "SDL2 window created successfuly");
    return false;
}
#endif
