#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#include <omega/window_raylib.h>
#include <omega/surface_raylib.h>
#include <omega/renderer_raylib.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define ren_raylib ((OMG_RendererRaylib*)base->ren)

bool omg_window_raylib_show(OMG_WindowRaylib* this, bool show) {
    (show ? this->raylib->ClearWindowState : this->raylib->SetWindowState)(FLAG_WINDOW_HIDDEN);
    return false;
}

bool omg_window_raylib_set_state(OMG_WindowRaylib* this, int state) {
    if (state & OMG_WIN_STATE_MINIMIZED) {
        state &= (~OMG_WIN_STATE_MAXIMIZED) | (~OMG_WIN_STATE_RESTORED);
        this->raylib->MinimizeWindow();
    }
    else if (state & OMG_WIN_STATE_MAXIMIZED) {
        state &= (~OMG_WIN_STATE_MINIMIZED) | (~OMG_WIN_STATE_RESTORED);
        this->raylib->MaximizeWindow();
    }
    else if (!(base->state & OMG_WIN_STATE_RESTORED)) {
        state = OMG_WIN_STATE_RESTORED;
        this->raylib->RestoreWindow();
    }
    base->state = state;
    return false;
}

bool omg_window_raylib_set_sys_button(OMG_WindowRaylib* this, int id, bool enabled) {
    OMG_UNUSED(this, id, enabled);
    return false;
}

bool omg_window_raylib_set_resizable(OMG_WindowRaylib* this, bool enabled) {
    base->resizable = enabled;
    if (enabled) {
        base->sys_buttons |= OMG_WIN_SYS_BUTTON_MAXIMIZE;
        this->raylib->SetWindowState(FLAG_WINDOW_RESIZABLE);
    }
    else {
        base->sys_buttons &= ~OMG_WIN_SYS_BUTTON_MAXIMIZE;
        this->raylib->ClearWindowState(FLAG_WINDOW_RESIZABLE);
    }
    return false;
}

bool omg_window_raylib_set_bordered(OMG_WindowRaylib* this, bool enabled) {
    base->bordered = enabled;
    if (enabled)
        this->raylib->ClearWindowState(FLAG_WINDOW_UNDECORATED);
    else
        this->raylib->SetWindowState(FLAG_WINDOW_UNDECORATED);
    return false;
}

bool omg_window_raylib_set_always_on_top(OMG_WindowRaylib* this, bool enabled) {
    base->always_on_top = enabled;
    if (enabled)
        this->raylib->SetWindowState(FLAG_WINDOW_TOPMOST);
    else
        this->raylib->ClearWindowState(FLAG_WINDOW_TOPMOST);
    return false;
}

bool omg_window_raylib_set_window_mode(OMG_WindowRaylib* this, int mode) {
    if (mode == OMG_WIN_MODE_WINDOW) {
        if (base->window_mode == OMG_WIN_MODE_FULLSCREEN)
            this->raylib->ToggleFullscreen();
        else if (base->window_mode == OMG_WIN_MODE_DESKTOP_FULLSCREEN)
            this->raylib->ToggleBorderlessWindowed();
    }
    else if (mode == OMG_WIN_MODE_DESKTOP_FULLSCREEN) {
        if (base->window_mode == OMG_WIN_MODE_WINDOW)
            this->raylib->ToggleBorderlessWindowed();
        else if (base->window_mode == OMG_WIN_MODE_DESKTOP_FULLSCREEN) {
            this->raylib->ToggleFullscreen();
            this->raylib->ToggleBorderlessWindowed();
        }
    }
    else if (mode == OMG_WIN_MODE_FULLSCREEN) {
        if (base->window_mode == OMG_WIN_MODE_WINDOW)
            this->raylib->ToggleFullscreen();
        else if (base->window_mode == OMG_WIN_MODE_DESKTOP_FULLSCREEN) {
            this->raylib->ToggleBorderlessWindowed();
            this->raylib->ToggleFullscreen();
        }
    }
    base->window_mode = mode;
    return false;
}

bool omg_window_raylib_set_size(OMG_WindowRaylib* this, const OMG_FPoint* new_size) {
    omg_window_set_size((OMG_Window*)this, new_size);
    this->raylib->SetWindowSize((int)new_size->w, (int)new_size->h);
    return false;
}

bool omg_window_raylib_destroy(OMG_WindowRaylib* this) {
    if (base->inited) {
        omg_window_destroy((OMG_Window*)this);
        this->raylib->CloseWindow();
        base->inited = false;
    }
    return false;
}

bool omg_window_raylib_set_title(OMG_WindowRaylib* this, const OMG_String* new_title) {
    if (omg_string_ensure_null((OMG_String*)new_title))
        return true;
    this->raylib->SetWindowTitle(new_title->ptr);
    return false;
}

bool omg_window_raylib_renderer_alloc(OMG_WindowRaylib* this) {
    base->ren_type = OMG_REN_TYPE_RAYLIB;
    base->ren = OMG_MALLOC(omg_base->mem, sizeof(OMG_RendererRaylib));
    if (OMG_ISNULL(base->ren))
        return true;
    omg_renderer_fill_on_create(base->ren);
    base->ren->was_allocated = true;
    base->ren->inited = false;
    base->ren->win = this;
    OMG_BEGIN_POINTER_CAST();
    base->ren->get_supported_drivers = omg_renderer_raylib_get_supported_drivers;
    base->ren->init = omg_renderer_raylib_init;
    OMG_END_POINTER_CAST();
    ren_raylib->raylib = this->raylib;
    return false;
}

bool omg_window_raylib_set_icon(OMG_WindowRaylib* this, OMG_SurfaceRaylib* icon) {
    this->raylib->SetWindowIcon(icon->img);
    return false;
}

bool omg_window_raylib_mouse_warp(OMG_WindowRaylib* this, const OMG_FPoint* pos) {
    this->raylib->SetMousePosition((int)pos->x, (int)pos->y);
    return false;
}

bool omg_window_raylib_mouse_set_rel(OMG_WindowRaylib* this, int rel_mode) {
    if (rel_mode == 0)
        this->raylib->EnableCursor();
    else if (rel_mode == 1)
        this->raylib->DisableCursor();
    else if (rel_mode == 2)
        (this->raylib->IsCursorHidden() ? this->raylib->EnableCursor : this->raylib->DisableCursor)();
    else
        return this->raylib->IsCursorHidden();
    return false;
}

bool omg_window_raylib_cursor_set_shown(OMG_WindowRaylib* this, int show_mode) {
    if (show_mode == 0)
        this->raylib->ShowCursor();
    else if (show_mode == 1)
        this->raylib->HideCursor();
    else if (show_mode == 2)
        (this->raylib->IsCursorHidden() ? this->raylib->ShowCursor : this->raylib->HideCursor)();
    else
        return this->raylib->IsCursorHidden();
    return false;
}

bool omg_window_raylib_mouse_set_system_cursor(OMG_WindowRaylib* this, int cursor_id) {
    MouseCursor cur;
    if (cursor_id == OMG_SYSTEM_CURSOR_ARROW)
        cur = MOUSE_CURSOR_ARROW;
    else if (cursor_id == OMG_SYSTEM_CURSOR_IBEAM)
        cur = MOUSE_CURSOR_IBEAM;
    else if (cursor_id == OMG_SYSTEM_CURSOR_SIZEWE)
        cur = MOUSE_CURSOR_RESIZE_EW;
    else if (cursor_id == OMG_SYSTEM_CURSOR_SIZENS)
        cur = MOUSE_CURSOR_RESIZE_NS;
    else if (cursor_id == OMG_SYSTEM_CURSOR_SIZENWSE)
        cur = MOUSE_CURSOR_RESIZE_NWSE;
    else if (cursor_id == OMG_SYSTEM_CURSOR_SIZENESW)
        cur = MOUSE_CURSOR_RESIZE_NESW;
    else if (cursor_id == OMG_SYSTEM_CURSOR_SIZEALL)
        cur = MOUSE_CURSOR_RESIZE_ALL;
    else if (cursor_id == OMG_SYSTEM_CURSOR_CROSSHAIR)
        cur = MOUSE_CURSOR_CROSSHAIR;
    else if (cursor_id == OMG_SYSTEM_CURSOR_HAND)
        cur = MOUSE_CURSOR_POINTING_HAND;
    else if (cursor_id == OMG_SYSTEM_CURSOR_NO)
        cur = MOUSE_CURSOR_NOT_ALLOWED;
    else
        cur = MOUSE_CURSOR_DEFAULT;
    this->raylib->SetMouseCursor(cur);
    return false;
}

int omg_window_raylib_display_get_index(OMG_WindowRaylib* this) {
    return this->raylib->GetCurrentMonitor();
}

bool omg_window_raylib_init(OMG_WindowRaylib* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_RAYLIB;
    base->inited = false;
    OMG_BEGIN_POINTER_CAST();
    base->show = omg_window_raylib_show;
    base->set_size = omg_window_set_size;
    base->set_icon = omg_window_raylib_set_icon;
    base->set_title = omg_window_raylib_set_title;
    base->destroy = omg_window_raylib_destroy;
    base->renderer_alloc = omg_window_raylib_renderer_alloc;
    base->set_state = omg_window_raylib_set_state;
    base->set_sys_button = omg_window_raylib_set_sys_button;
    base->set_resizable = omg_window_raylib_set_resizable;
    base->set_bordered = omg_window_raylib_set_bordered;
    base->set_always_on_top = omg_window_raylib_set_always_on_top;
    base->set_window_mode = omg_window_raylib_set_window_mode;
    base->mouse_warp = omg_window_raylib_mouse_warp;
    base->mouse_set_rel = omg_window_raylib_mouse_set_rel;
    base->cursor_set_shown = omg_window_raylib_cursor_set_shown;
    base->mouse_set_system_cursor = omg_window_raylib_mouse_set_system_cursor;
    base->display_get_index = omg_window_raylib_display_get_index;
    OMG_END_POINTER_CAST();
    this->raylib->SetConfigFlags(
        FLAG_WINDOW_HIDDEN |
        (omg_base->support_highdpi ? FLAG_WINDOW_HIGHDPI : 0) |
        (base->vsync ? FLAG_VSYNC_HINT : 0) |
        (base->aa ? FLAG_MSAA_4X_HINT : 0) |
        (base->resizable ? FLAG_WINDOW_RESIZABLE : 0) |
        (base->bordered ? 0 : FLAG_WINDOW_UNDECORATED) |
        (base->always_on_top ? FLAG_WINDOW_TOPMOST : 0) |
        ((base->state & OMG_WIN_STATE_MINIMIZED) ? FLAG_WINDOW_MINIMIZED : 0) |
        ((base->state & OMG_WIN_STATE_MAXIMIZED) ? FLAG_WINDOW_MAXIMIZED : 0) |
        ((base->state & OMG_WIN_STATE_RESTORED) ? 0 : FLAG_WINDOW_UNFOCUSED) |
        FLAG_WINDOW_ALWAYS_RUN
    );
    this->raylib->InitWindow((int)base->size.w, (int)base->size.h, "OMG Window [Raylib]");
    base->inited = this->raylib->IsWindowReady();
    if (base->inited) {
        if (base->window_mode == OMG_WIN_MODE_DESKTOP_FULLSCREEN)
            this->raylib->ToggleBorderlessWindowed();
        else if (base->window_mode == OMG_WIN_MODE_FULLSCREEN)
            this->raylib->ToggleFullscreen();
        _OMG_LOG_INFO(omg_base, "Raylib window created successfuly");
    }
    return !base->inited;
}
#endif
