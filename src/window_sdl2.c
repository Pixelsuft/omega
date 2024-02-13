#include <omega/window_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/omega.h>
#include <omega/surface_sdl2.h>
#include <omega/winmgr_sdl2.h>
#include <omega/renderer_sdl2.h>
#include <omega/config.h>
#if OMG_IS_WIN && (!OMG_SDL2_DYNAMIC)
#if OMG_IS_VC || OMG_IS_UNIX || OMG_IS_EMSCRIPTEN || OMG_IS_ANDROID
#include <SDL_syswm.h>
#else
#include <SDL2/SDL_syswm.h>
#endif
#endif
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define winmgr_sdl2 ((OMG_WinmgrSdl2*)omg_base->winmgr)
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
        ren_sdl2->win_build_num = 0; // TODO
        base->ren->was_allocated = true;
        base->ren->inited = false;
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
    if (enabled)
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
    base->window_mode = mode;
    return false;
}

bool omg_window_sdl2_set_size(OMG_WindowSdl2* this, const OMG_FPoint* new_size) {
    omg_window_set_size((OMG_Window*)this, new_size);
    this->sdl2->SDL_SetWindowSize(this->win, (int)new_size->w, (int)new_size->h);
    return false;
}

bool omg_window_sdl2_set_min_size(OMG_WindowSdl2* this, const OMG_FPoint* new_min_size) {
    omg_window_set_min_size((OMG_Window*)this, new_min_size);
    this->sdl2->SDL_SetWindowMinimumSize(this->win, (int)new_min_size->w, (int)new_min_size->h);
    return false;
}

bool omg_window_sdl2_set_max_size(OMG_WindowSdl2* this, const OMG_FPoint* new_max_size) {
    omg_window_set_max_size((OMG_Window*)this, new_max_size);
    this->sdl2->SDL_SetWindowMaximumSize(this->win, (int)new_max_size->w, (int)new_max_size->h);
    return false;
}

bool omg_window_sdl2_set_icon(OMG_WindowSdl2* this, OMG_SurfaceSdl2* icon) {
    this->sdl2->SDL_SetWindowIcon(this->win, icon->surf);
    return false;
}

bool omg_window_sdl2_mouse_warp(OMG_WindowSdl2* this, const OMG_FPoint* pos) {
    this->sdl2->SDL_WarpMouseInWindow(this->win, (int)pos->x, (int)pos->y);
    return false;
}

bool omg_window_sdl2_mouse_set_rel(OMG_WindowSdl2* this, int rel_mode) {
    int res;
    if (rel_mode == 0)
        res = this->sdl2->SDL_SetRelativeMouseMode(SDL_FALSE);
    else if (rel_mode == 1)
        res = this->sdl2->SDL_SetRelativeMouseMode(SDL_TRUE);
    else if (rel_mode == 2)
        res = this->sdl2->SDL_SetRelativeMouseMode(this->sdl2->SDL_GetRelativeMouseMode() ? SDL_FALSE : SDL_TRUE);
    else {
        return this->sdl2->SDL_GetRelativeMouseMode() ? true : false;
    }
    if (res < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set relative mouse mode for window (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    return false;
}

bool omg_window_sdl2_set_grab(OMG_WindowSdl2* this, int grab_mode) {
    if (grab_mode == 0)
        this->sdl2->SDL_SetWindowGrab(this->win, SDL_FALSE);
    else if (grab_mode == 1)
        this->sdl2->SDL_SetWindowGrab(this->win, SDL_TRUE);
    else if (grab_mode == 2)
        this->sdl2->SDL_SetWindowGrab(this->win, this->sdl2->SDL_GetWindowGrab(this->win) ? SDL_FALSE : SDL_TRUE);
    else {
        return this->sdl2->SDL_GetWindowGrab(this->win) ? true : false;
    }
    return false;
}

bool omg_window_sdl2_cursor_set_shown(OMG_WindowSdl2* this, int show_mode) {
    int res;
    if (show_mode == 0)
        res = this->sdl2->SDL_ShowCursor(SDL_DISABLE);
    else if (show_mode == 1)
        res = this->sdl2->SDL_ShowCursor(SDL_ENABLE);
    else if (show_mode == 2)
        res = this->sdl2->SDL_ShowCursor((this->sdl2->SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE) ? SDL_DISABLE : SDL_ENABLE);
    else {
        return this->sdl2->SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
    }
    if (res < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set cursor show mode for window (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    return false;
}

bool omg_window_sdl2_mouse_set_system_cursor(OMG_WindowSdl2* this, int cursor_id) {
    SDL_Cursor* cur;
    if (cursor_id < 0)
        cur = this->sdl2->SDL_GetDefaultCursor();
    else
        cur = this->sdl2->SDL_CreateSystemCursor((SDL_SystemCursor)cursor_id);
    if (OMG_ISNULL(cur)) {
        _OMG_LOG_WARN(omg_base, "Failed to set system cursor (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    if (OMG_ISNOTNULL(winmgr_sdl2->cursor_cache)) {
        this->sdl2->SDL_FreeCursor(winmgr_sdl2->cursor_cache);
    }
    winmgr_sdl2->cursor_cache = cur;
    this->sdl2->SDL_SetCursor(cur);
    return false;
}

int omg_window_sdl2_display_get_index(OMG_WindowSdl2* this) {
    int res = this->sdl2->SDL_GetWindowDisplayIndex(this->win);
    if (res < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to get window display index (", this->sdl2->SDL_GetError(), ")");
        return omg_window_display_get_index(base);
    }
    return res;
}

bool omg_window_sdl2_display_get_mode(OMG_WindowSdl2* this, OMG_VideoMode* mode) {
    SDL_DisplayMode sdm;
    if (this->sdl2->SDL_GetWindowDisplayMode(this->win, &sdm) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to get window display mode (", this->sdl2->SDL_GetError(), ")");
        return omg_window_display_get_mode(base, mode);
    }
    mode->rate = (float)sdm.refresh_rate;
    mode->size.w = (float)sdm.w;
    mode->size.h = (float)sdm.h;
    return false;
}

bool omg_window_sdl2_display_set_mode(OMG_WindowSdl2* this, const OMG_VideoMode* mode) {
    SDL_DisplayMode sdm;
    sdm.refresh_rate = (int)mode->rate;
    sdm.w = (int)mode->size.w;
    sdm.h = (int)mode->size.h;
    if (this->sdl2->SDL_SetWindowDisplayMode(this->win, &sdm) < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set window display mode (", this->sdl2->SDL_GetError(), ")");
        return omg_window_display_set_mode(base, mode);
    }
    return false;
}

bool omg_window_sdl2_get_pos(OMG_WindowSdl2* this, OMG_FRect* pos) {
    int x_buf, y_buf;
    this->sdl2->SDL_GetWindowPosition(this->win, &x_buf, &y_buf);
    pos->x = (float)x_buf;
    pos->y = (float)y_buf;
    return false;
}

bool omg_window_sdl2_set_pos(OMG_WindowSdl2* this, const OMG_FRect* pos) {
    this->sdl2->SDL_SetWindowPosition(this->win, (int)pos->x, (int)pos->y);
    return false;
}

bool omg_window_sdl2_raise(OMG_WindowSdl2* this) {
    this->sdl2->SDL_RaiseWindow(this->win);
    return false;
}

bool omg_window_sdl2_mouse_set_rect(OMG_WindowSdl2* this, const OMG_FRect* rect) {
    int res;
    if (OMG_ISNULL(rect))
        res = this->sdl2->SDL_SetWindowMouseRect(this->win, NULL);
    else {
        SDL_Rect sdl_rect;
        sdl_rect.x = (int)rect->x;
        sdl_rect.y = (int)rect->y;
        sdl_rect.w = (int)rect->w;
        sdl_rect.h = (int)rect->h;
        res = this->sdl2->SDL_SetWindowMouseRect(this->win, &sdl_rect);
    }
    if (res < 0) {
        _OMG_LOG_WARN(omg_base, "Failed to set window mouse rect (", this->sdl2->SDL_GetError(), ")");
        return omg_window_mouse_set_rect(base, rect);
    }
    return false;
}

bool omg_window_sdl2_set_brightness(OMG_WindowSdl2* this, float brightness) {
    if (this->sdl2->SDL_SetWindowBrightness(this->win, brightness) < 0) {
        _OMG_LOG_INFO(omg_base, "Failed to set window brightness (", this->sdl2->SDL_GetError(), ")");
        return omg_window_set_brightness(base, brightness);
    }
    return false;
}

float omg_window_sdl2_get_brightness(OMG_WindowSdl2* this) {
    return this->sdl2->SDL_GetWindowBrightness(this->win);
}

bool omg_window_sdl2_set_opacity(OMG_WindowSdl2* this, float opacity) {
    if (OMG_ISNULL(this->sdl2->SDL_SetWindowOpacity))
        return omg_window_set_opacity(base, opacity);
    if (this->sdl2->SDL_SetWindowOpacity(this->win, opacity) < 0) {
        _OMG_LOG_INFO(omg_base, "Failed to set window opacity (", this->sdl2->SDL_GetError(), ")");
        return omg_window_set_opacity(base, opacity);
    }
    return false;
}

float omg_window_sdl2_get_opacity(OMG_WindowSdl2* this) {
    if (OMG_ISNULL(this->sdl2->SDL_GetWindowOpacity))
        return omg_window_get_opacity(base);
    float res = -1.0f;
    if (this->sdl2->SDL_GetWindowOpacity(this->win, &res) < 0) {
        _OMG_LOG_INFO(omg_base, "Failed to get window opacity (", this->sdl2->SDL_GetError(), ")");
        return omg_window_get_opacity(base);
    }
    return res;
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
    this->sdl2->SDL_SetWindowData(this->win, "a", this);
#if OMG_IS_WIN
    SDL_SysWMinfo wm_info;
    wm_info.version.major = 2;
    wm_info.version.minor = 31;
    wm_info.version.patch = 0;
    if (this->sdl2->SDL_GetWindowWMInfo(this->win, &wm_info) == SDL_TRUE) {
        base->win32_handle = (void*)wm_info.win.window;
    }
    else {
        base->win32_handle = NULL;
        _OMG_LOG_WARN(omg_base, "Failed to get window manager info (", this->sdl2->SDL_GetError(), ")");
    }
#endif
    OMG_BEGIN_POINTER_CAST();
    base->raise = omg_window_sdl2_raise;
    base->set_size = omg_window_sdl2_set_size;
    base->set_min_size = omg_window_sdl2_set_min_size;
    base->set_max_size = omg_window_sdl2_set_max_size;
    base->set_state = omg_window_sdl2_set_title;
    base->set_sys_button = omg_window_sdl2_set_title;
    base->set_resizable = omg_window_sdl2_set_resizable;
    base->set_bordered = omg_window_sdl2_set_bordered;
    base->set_always_on_top = omg_window_sdl2_set_always_on_top;
    base->set_window_mode = omg_window_sdl2_set_window_mode;
    base->renderer_alloc = omg_window_sdl2_renderer_alloc;
    base->show = omg_window_sdl2_show;
    base->set_icon = omg_window_sdl2_set_icon;
    base->set_title = omg_window_sdl2_set_title;
    base->mouse_warp = omg_window_sdl2_mouse_warp;
    base->mouse_set_rel = omg_window_sdl2_mouse_set_rel;
    base->cursor_set_shown = omg_window_sdl2_cursor_set_shown;
    base->mouse_set_system_cursor = omg_window_sdl2_mouse_set_system_cursor;
    base->set_grab = omg_window_sdl2_set_grab;
    base->display_get_index = omg_window_sdl2_display_get_index;
    base->display_get_mode = omg_window_sdl2_display_get_mode;
    base->display_set_mode = omg_window_sdl2_display_set_mode;
    base->mouse_set_rect = omg_window_sdl2_mouse_set_rect;
    base->get_pos = omg_window_sdl2_get_pos;
    base->set_pos = omg_window_sdl2_set_pos;
    base->set_brightness = omg_window_sdl2_set_brightness;
    base->get_brightness = omg_window_sdl2_get_brightness;
    base->set_opacity = omg_window_sdl2_set_opacity;
    base->get_opacity = omg_window_sdl2_get_opacity;
    base->destroy = omg_window_sdl2_destroy;
    OMG_END_POINTER_CAST();
    base->inited = true;
    _OMG_LOG_INFO(omg_base, "SDL2 window created successfuly");
    return false;
}
#endif
