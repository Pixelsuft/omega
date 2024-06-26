#include <omega/omega.h>
#include <omega/window.h>
#include <omega/api_win.h>
#define omg_base ((OMG_Omega*)this->omg)
#define d_uxtheme ((OMG_Uxtheme*)omg_base->uxtheme)
#define d_dwm ((OMG_Dwmapi*)omg_base->dwm)
#define d_u32 ((OMG_User32*)omg_base->u32)
#define d_k32 ((OMG_Kernel32*)omg_base->k32)

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
        if (this->ren->inited)
            this->ren->destroy(this->ren);
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

bool omg_window_set_always_on_top(OMG_Window* this, bool enabled) {
    this->always_on_top = enabled;
    return false;
}

bool omg_window_set_window_mode(OMG_Window* this, int mode) {
    this->window_mode = mode;
    return false;
}

bool omg_window_set_size(OMG_Window* this, const OMG_FPoint* new_size) {
    this->size.w = new_size->w;
    this->size.h = new_size->h;
    return false;
}

bool omg_window_set_min_size(OMG_Window* this, const OMG_FPoint* new_min_size) {
    this->min_size.w = new_min_size->w;
    this->min_size.h = new_min_size->h;
    return false;
}

bool omg_window_set_max_size(OMG_Window* this, const OMG_FPoint* new_max_size) {
    this->max_size.w = new_max_size->w;
    this->max_size.h = new_max_size->h;
    return false;
}

bool omg_window_set_icon(OMG_Window* this, OMG_Surface* icon) {
    OMG_UNUSED(this, icon);
    return false;
}

bool omg_window_mouse_warp(OMG_Window* this, const OMG_FPoint* pos) {
    OMG_UNUSED(this, pos);
    return true;
}

bool omg_window_mouse_set_rel(OMG_Window* this, int rel_mode) {
    OMG_UNUSED(this, rel_mode);
    return (rel_mode == 1) || (rel_mode == 2);
}

bool omg_window_set_grab(OMG_Window* this, int grab_mode) {
    OMG_UNUSED(this, grab_mode);
    return (grab_mode == 1) || (grab_mode == 2);
}

bool omg_window_cursor_set_shown(OMG_Window* this, int show_mode) {
    OMG_UNUSED(this, show_mode);
    return (show_mode == 1) || (show_mode == 2);
}

bool omg_window_mouse_set_system_cursor(OMG_Window* this, int cursor_id) {
    OMG_UNUSED(this, cursor_id);
    return true;
}

int omg_window_display_get_index(OMG_Window* this) {
    OMG_UNUSED(this);
    return -1;
}

bool omg_window_display_get_mode(OMG_Window* this, OMG_VideoMode* mode) {
    int disp = this->display_get_index(this);
    if (disp < 0) {
        mode->rate = 0.0f;
        mode->size.w = mode->size.h = 0.0f;
        return true;
    }
    return omg_base->winmgr->display_get_current_mode(omg_base->winmgr, disp, mode);
}

bool omg_window_display_set_mode(OMG_Window* this, const OMG_VideoMode* mode) {
    return this->set_size(this, &mode->size);
}

bool omg_window_get_pos(OMG_Window* this, OMG_FRect* pos) {
    OMG_UNUSED(this);
    pos->x = pos->y = 0.0f;
    return true;
}

bool omg_window_set_pos(OMG_Window* this, const OMG_FRect* pos) {
    OMG_UNUSED(this, pos);
    return true;
}

bool omg_window_raise(OMG_Window* this) {
    OMG_UNUSED(this);
    return true;
}

bool omg_window_mouse_set_rect(OMG_Window* this, const OMG_FRect* rect) {
    return this->set_grab(this, OMG_ISNOTNULL(rect));
}

bool omg_window_set_brightness(OMG_Window* this, float brightness) {
    OMG_UNUSED(this, brightness);
    return true;
}

float omg_window_get_brightness(OMG_Window* this) {
    OMG_UNUSED(this);
    return -1.0f;
}

bool omg_window_set_opacity(OMG_Window* this, float opacity) {
    OMG_UNUSED(this, opacity);
    return true;
}

float omg_window_get_opacity(OMG_Window* this) {
    OMG_UNUSED(this);
    return -1.0f;
}

void omg_window_win_check_dark_mode(OMG_Window* this) {
#if OMG_IS_WIN
    if (OMG_ISNOTNULL(d_uxtheme->ShouldAppsUseDarkMode) && (true || (omg_base->theme == OMG_THEME_NONE))) {
        omg_base->theme = d_uxtheme->ShouldAppsUseDarkMode() ? OMG_THEME_DARK : OMG_THEME_LIGHT;
    }
    if (OMG_ISNULL(this->win32_handle))
        return;
    if (OMG_ISNOTNULL(d_uxtheme->AllowDarkModeForWindow)) {
        d_uxtheme->AllowDarkModeForWindow(
            (HWND)this->win32_handle,
            ((omg_base->app_theme == OMG_THEME_DARK) || (omg_base->app_theme == OMG_THEME_AUTO)) && (omg_base->theme != OMG_THEME_LIGHT)
        );
    }
    if (OMG_ISNOTNULL(d_dwm->DwmSetWindowAttribute)) {
        DWORD val = (omg_base->theme == OMG_THEME_DARK) ? 1 : 0;
        d_dwm->DwmSetWindowAttribute((HWND)this->win32_handle, 19, &val, sizeof(DWORD));
        val = (omg_base->theme == OMG_THEME_DARK) ? 1 : 0;
        d_dwm->DwmSetWindowAttribute((HWND)this->win32_handle, 20, &val, sizeof(DWORD));
    }
#else
    OMG_UNUSED(this);
#endif
}

bool omg_window_message_box(OMG_Window* this, const OMG_String* text, const OMG_String* title, int flags) {
#if OMG_IS_WIN
    size_t count1;
    size_t count2;
    char* title_ptr;
    _OMG_MSGBOX_DEF_FILL_TITLE(title, title_ptr);
    size_t title_ptr_len = OMG_ISNULL(title) ? omg_base->std->strlen(title_ptr) : title->len;
    _OMG_WIN_GET_ENCODE_SIZE(count1, text, d_k32);
    count2 = title_ptr_len * 2;
    if (count1 == 0) {
        return true;
    }
    wchar_t* w_o_fp = OMG_MALLOC(omg_base->mem, (size_t)(count1 + count2) * 2 + 4);
    if (OMG_ISNULL(w_o_fp))
        return true;
    wchar_t* w_n_fp = (wchar_t*)((size_t)w_o_fp + ((size_t)(count1) * 2) + 2);
    int out_len_o = d_k32->MultiByteToWideChar(CP_UTF8, 0, text->ptr, (int)text->len, w_o_fp, (int)count1);
    if (out_len_o > 0)
        w_o_fp[out_len_o] = L'\0';
    int out_len_n = d_k32->MultiByteToWideChar(CP_UTF8, 0, title_ptr, (int)title_ptr_len, w_n_fp, (int)count2);
    if (out_len_n > 0)
        w_n_fp[out_len_n] = L'\0';
    UINT win_flags = 0;
    if (flags & OMG_MESSAGEBOX_INFO)
        win_flags |= MB_ICONINFORMATION;
    if (flags & OMG_MESSAGEBOX_WARN)
        win_flags |= MB_ICONWARNING;
    if (flags & OMG_MESSAGEBOX_ERROR)
        win_flags |= MB_ICONERROR;
    bool res = false;
    if (d_u32->MessageBoxW((HWND)this->win32_handle, w_o_fp, w_n_fp, win_flags) == 0) {
        res = true;
    }
    OMG_FREE(omg_base->mem, w_o_fp);
    return res;
#else
    OMG_UNUSED(this, text, title, flags);
    return true;
#endif
}

bool omg_window_init(OMG_Window* this) {
    if (this->sys_buttons < 0) {
        this->sys_buttons = OMG_WIN_SYS_BUTTON_CLOSE | OMG_WIN_SYS_BUTTON_MINIMIZE | (this->resizable ? OMG_WIN_SYS_BUTTON_MAXIMIZE : 0);
    }
    this->scale.x = this->scale.y = 1.0f;
    this->min_size.w = this->min_size.h = 0.0f;
    this->max_size.w = this->max_size.h = 0.0f;
    this->ren = NULL;
    this->win32_handle = NULL;
    this->allow_alt = true;
    this->allow_alt_f4 = true;
    this->enable_paint = true;
    this->ren_type = OMG_REN_TYPE_AUTO;
    this->destroy = omg_window_destroy;
    this->set_size = omg_window_set_size;
    this->set_min_size = omg_window_set_min_size;
    this->set_max_size = omg_window_set_max_size;
    this->set_state = omg_window_set_state;
    this->raise = omg_window_raise;
    this->set_sys_button = omg_window_set_sys_button;
    this->set_resizable = omg_window_set_resizable;
    this->set_bordered = omg_window_set_bordered;
    this->set_always_on_top = omg_window_set_always_on_top;
    this->show = omg_window_show;
    this->set_title = omg_window_set_title;
    this->renderer_alloc = omg_window_renderer_alloc;
    this->renderer_free = omg_window_renderer_free;
    this->set_window_mode = omg_window_set_window_mode;
    this->set_icon = omg_window_set_icon;
    this->mouse_warp = omg_window_mouse_warp;
    this->mouse_set_rel = omg_window_mouse_set_rel;
    this->cursor_set_shown = omg_window_cursor_set_shown;
    this->mouse_set_system_cursor = omg_window_mouse_set_system_cursor;
    this->set_grab = omg_window_set_grab;
    this->display_get_index = omg_window_display_get_index;
    this->display_get_mode = omg_window_display_get_mode;
    this->display_set_mode = omg_window_display_set_mode;
    this->get_pos = omg_window_get_pos;
    this->set_pos = omg_window_set_pos;
    this->set_brightness = omg_window_set_brightness;
    this->get_brightness = omg_window_get_brightness;
    this->set_opacity = omg_window_set_opacity;
    this->get_opacity = omg_window_get_opacity;
    this->message_box = omg_window_message_box;
    for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) {
        if (OMG_ISNULL(omg_base->winmgr->cache[i])) {
            omg_base->winmgr->cache[i] = this;
            this->array_pos = i;
            break;
        }
    }
    return false;
}
