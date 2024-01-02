#include <omega/window_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega_win.h>
#include <omega/renderer_sdl2.h>
#define base ((OMG_Window*)this)
#define omg_base ((OMG_Omega*)base->omg)
#define ren_sdl2 ((OMG_RendererSdl2*)base->ren)
#define RET_DEF_PROC() this->u32->DefWindowProcW(hwnd, msg, wparam, lparam)
#ifdef SetWindowLongPtrW
#define GET_WIN_STYLE() (int64_t)this->u32->GetWindowLongW(this->hwnd, GWL_STYLE)
#define SET_WIN_STYLE(value) this->u32->SetWindowLongW(this->hwnd, GWL_STYLE, (LONG)(value))
#else
#define GET_WIN_STYLE() (int64_t)this->u32->GetWindowLongPtrW(this->hwnd, GWL_STYLE)
#define SET_WIN_STYLE(value) this->u32->SetWindowLongPtrW(this->hwnd, GWL_STYLE, (LONG_PTR)(value))
#endif
#define KEYBOARD_FILL_MOD(mod) do { \
    mod = 0; \
    if (omg_base->keyboard_state[OMG_SCANCODE_LCTRL]) \
        mod |= OMG_KMOD_LCTRL; \
    if (omg_base->keyboard_state[OMG_SCANCODE_RCTRL]) \
        mod |= OMG_KMOD_RCTRL; \
    if (omg_base->keyboard_state[OMG_SCANCODE_LSHIFT]) \
        mod |= OMG_KMOD_LSHIFT; \
    if (omg_base->keyboard_state[OMG_SCANCODE_RSHIFT]) \
        mod |= OMG_KMOD_RSHIFT; \
    if (omg_base->keyboard_state[OMG_SCANCODE_LALT]) \
        mod |= OMG_KMOD_LALT; \
    if (omg_base->keyboard_state[OMG_SCANCODE_RALT]) \
        mod |= OMG_KMOD_RALT; \
    if (omg_base->keyboard_state[OMG_SCANCODE_LGUI]) \
        mod |= OMG_KMOD_LGUI; \
    if (omg_base->keyboard_state[OMG_SCANCODE_RGUI]) \
        mod |= OMG_KMOD_RGUI; \
} while (0)
#define MOUSE_FILL_STATE(state, wparam) do { \
    state = 0; \
    if (wparam & MK_LBUTTON) \
        state |= OMG_MBUTTON_LMASK; \
    if (wparam & MK_MBUTTON) \
        state |= OMG_MBUTTON_MMASK; \
    if (wparam & MK_RBUTTON) \
        state |= OMG_MBUTTON_RMASK; \
    if (wparam & MK_XBUTTON1) \
        state |= OMG_MBUTTON_X1MASK; \
    if (wparam & MK_XBUTTON2) \
        state |= OMG_MBUTTON_X2MASK; \
} while (0)
#define MAKE_EVENT(event) do { \
    ((OMG_Event*)event)->omg = base->omg; \
    ((OMG_Event*)event)->data = omg_base->event_arg; \
    ((OMG_Event*)event)->time = (uint64_t)((OMG_OmegaWin*)omg_base)->msg.time; \
} while (0)

static LONG OMG_WIN_STD_PREFIX (*OMG_WIN_CB_GetWindowLongW)(HWND, int) = NULL;
static LONG_PTR OMG_WIN_STD_PREFIX (*OMG_WIN_CB_GetWindowLongPtrW)(HWND, int) = NULL;
// Thanks to SDL2
static const OMG_Scancode OMG_windows_scancode_table[] = {
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_ESCAPE,
	OMG_SCANCODE_1,
	OMG_SCANCODE_2,
	OMG_SCANCODE_3,
	OMG_SCANCODE_4,
	OMG_SCANCODE_5,
	OMG_SCANCODE_6,
	OMG_SCANCODE_7,
	OMG_SCANCODE_8,
	OMG_SCANCODE_9,
	OMG_SCANCODE_0,
	OMG_SCANCODE_MINUS,
	OMG_SCANCODE_EQUALS,
	OMG_SCANCODE_BACKSPACE,
	OMG_SCANCODE_TAB,
	OMG_SCANCODE_Q,
	OMG_SCANCODE_W,
	OMG_SCANCODE_E,
	OMG_SCANCODE_R,
	OMG_SCANCODE_T,
	OMG_SCANCODE_Y,
	OMG_SCANCODE_U,
	OMG_SCANCODE_I,
	OMG_SCANCODE_O,
	OMG_SCANCODE_P,
	OMG_SCANCODE_LEFTBRACKET,
	OMG_SCANCODE_RIGHTBRACKET,
	OMG_SCANCODE_RETURN,
	OMG_SCANCODE_LCTRL,
	OMG_SCANCODE_A,
	OMG_SCANCODE_S,
	OMG_SCANCODE_D,
	OMG_SCANCODE_F,
	OMG_SCANCODE_G,
	OMG_SCANCODE_H,
	OMG_SCANCODE_J,
	OMG_SCANCODE_K,
	OMG_SCANCODE_L,
	OMG_SCANCODE_SEMICOLON,
	OMG_SCANCODE_APOSTROPHE,
	OMG_SCANCODE_GRAVE,
	OMG_SCANCODE_LSHIFT,
	OMG_SCANCODE_BACKSLASH,
	OMG_SCANCODE_Z,
	OMG_SCANCODE_X,
	OMG_SCANCODE_C,
	OMG_SCANCODE_V,
	OMG_SCANCODE_B,
	OMG_SCANCODE_N,
	OMG_SCANCODE_M,
	OMG_SCANCODE_COMMA,
	OMG_SCANCODE_PERIOD,
	OMG_SCANCODE_SLASH,
	OMG_SCANCODE_RSHIFT,
	OMG_SCANCODE_KP_MULTIPLY,
	OMG_SCANCODE_LALT,
	OMG_SCANCODE_SPACE,
	OMG_SCANCODE_CAPSLOCK,
	OMG_SCANCODE_F1,
	OMG_SCANCODE_F2,
	OMG_SCANCODE_F3,
	OMG_SCANCODE_F4,
	OMG_SCANCODE_F5,
	OMG_SCANCODE_F6,
	OMG_SCANCODE_F7,
	OMG_SCANCODE_F8,
	OMG_SCANCODE_F9,
	OMG_SCANCODE_F10,
	OMG_SCANCODE_NUMLOCKCLEAR,
	OMG_SCANCODE_SCROLLLOCK,
	OMG_SCANCODE_KP_7,
	OMG_SCANCODE_KP_8,
	OMG_SCANCODE_KP_9,
	OMG_SCANCODE_KP_MINUS,
	OMG_SCANCODE_KP_4,
	OMG_SCANCODE_KP_5,
	OMG_SCANCODE_KP_6,
	OMG_SCANCODE_KP_PLUS,
	OMG_SCANCODE_KP_1,
	OMG_SCANCODE_KP_2,
	OMG_SCANCODE_KP_3,
	OMG_SCANCODE_KP_0,
	OMG_SCANCODE_KP_PERIOD,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_NONUSBACKSLASH,
	OMG_SCANCODE_F11,
	OMG_SCANCODE_F12,
	OMG_SCANCODE_KP_EQUALS,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_INTERNATIONAL6,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_F13,
	OMG_SCANCODE_F14,
	OMG_SCANCODE_F15,
	OMG_SCANCODE_F16,
	OMG_SCANCODE_F17,
	OMG_SCANCODE_F18,
	OMG_SCANCODE_F19,
	OMG_SCANCODE_F20,
	OMG_SCANCODE_F21,
	OMG_SCANCODE_F22,
	OMG_SCANCODE_F23,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_INTERNATIONAL2,
	OMG_SCANCODE_LANG2,
	OMG_SCANCODE_LANG1,
	OMG_SCANCODE_INTERNATIONAL1,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_F24,
	OMG_SCANCODE_LANG4,
	OMG_SCANCODE_LANG3,
	OMG_SCANCODE_INTERNATIONAL4,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_INTERNATIONAL5,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_INTERNATIONAL3,
	OMG_SCANCODE_KP_COMMA,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_AUDIOPREV,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_AUDIONEXT,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_KP_ENTER,
	OMG_SCANCODE_RCTRL,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_MUTE,
	OMG_SCANCODE_CALCULATOR,
	OMG_SCANCODE_AUDIOPLAY,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_AUDIOSTOP,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_VOLUMEDOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_VOLUMEUP,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_AC_HOME,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_KP_DIVIDE,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_PRINTSCREEN,
	OMG_SCANCODE_RALT,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_NUMLOCKCLEAR,
	OMG_SCANCODE_PAUSE,
	OMG_SCANCODE_HOME,
	OMG_SCANCODE_UP,
	OMG_SCANCODE_PAGEUP,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_LEFT,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_RIGHT,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_END,
	OMG_SCANCODE_DOWN,
	OMG_SCANCODE_PAGEDOWN,
	OMG_SCANCODE_INSERT,
	OMG_SCANCODE_DELETE,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_LGUI,
	OMG_SCANCODE_RGUI,
	OMG_SCANCODE_APPLICATION,
	OMG_SCANCODE_POWER,
	OMG_SCANCODE_SLEEP,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_AC_SEARCH,
	OMG_SCANCODE_AC_BOOKMARKS,
	OMG_SCANCODE_AC_REFRESH,
	OMG_SCANCODE_AC_STOP,
	OMG_SCANCODE_AC_FORWARD,
	OMG_SCANCODE_AC_BACK,
	OMG_SCANCODE_COMPUTER,
	OMG_SCANCODE_MAIL,
	OMG_SCANCODE_MEDIASELECT,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN,
	OMG_SCANCODE_UNKNOWN
};

static void omg_win_windows_check_button_wparam(OMG_WindowWin* this, bool mouse_pressed, uint32_t mouse_flags, bool swap_buttons, uint8_t button, uint32_t mouse_id)
{
    if (swap_buttons) {
        if (button == OMG_MBUTTON_LEFT) {
            button = OMG_MBUTTON_RIGHT;
        } else if (button == OMG_MBUTTON_RIGHT) {
            button = OMG_MBUTTON_LEFT;
        }
    }

    if (mouse_pressed && !(mouse_flags & OMG_MBUTTON(button))) {
        OMG_EventMouseButton event;
        MAKE_EVENT(&event);
        event.is_emulated = false;
        event.is_pressed = true;
        event.win = this;
        event.id = mouse_id;
        event.button = button;
        event.pos.x = (float)this->mouse_pos_cache.x;
        event.pos.y = (float)this->mouse_pos_cache.y;
        event.clicks = 1;
        event.state = this->last_mouse_state;
        omg_base->on_mouse_down(&event);
    } else if (!mouse_pressed && (mouse_flags & OMG_MBUTTON(button))) {
        OMG_EventMouseButton event;
        MAKE_EVENT(&event);
        event.is_emulated = false;
        event.is_pressed = false;
        event.win = this;
        event.id = mouse_id;
        event.button = button;
        event.pos.x = (float)this->mouse_pos_cache.x;
        event.pos.y = (float)this->mouse_pos_cache.y;
        event.clicks = 1;
        event.state = this->last_mouse_state;
        omg_base->on_mouse_up(&event);
    }
}

static void omg_win_windows_check_mouse_buttons(OMG_WindowWin* this, WPARAM wparam, uint32_t mouse_id)
{
    if (wparam != this->last_mouse_state) {
        uint32_t mouse_flags = this->mouse_state_cache;
        this->last_mouse_state = wparam;
        omg_win_windows_check_button_wparam(this, (wparam & MK_LBUTTON), mouse_flags, false, OMG_MBUTTON_LEFT, mouse_id);
        omg_win_windows_check_button_wparam(this, (wparam & MK_MBUTTON), mouse_flags, false, OMG_MBUTTON_MIDDLE, mouse_id);
        omg_win_windows_check_button_wparam(this, (wparam & MK_RBUTTON), mouse_flags, false, OMG_MBUTTON_RIGHT, mouse_id);
        omg_win_windows_check_button_wparam(this, (wparam & MK_XBUTTON1), mouse_flags, false, OMG_MBUTTON_X1, mouse_id);
        omg_win_windows_check_button_wparam(this, (wparam & MK_XBUTTON2), mouse_flags, false, OMG_MBUTTON_X2, mouse_id);
    }
}

bool omg_window_win_show(OMG_WindowWin* this, bool show) {
    this->u32->ShowWindow(this->hwnd, show ? SW_SHOWNORMAL : SW_HIDE);
    return false;
}

static OMG_Scancode omg_window_win_code_to_scancode(OMG_WindowWin* this, LPARAM lparam, WPARAM wparam)
{
    // Thanks to SDL2
    OMG_Scancode code;
    uint8_t index;
    uint16_t keyFlags = HIWORD(lparam);
    uint16_t scanCode = LOBYTE(keyFlags);
    scanCode &= ~0x80;
    if (scanCode != 0) {
        if ((keyFlags & KF_EXTENDED) == KF_EXTENDED) {
            scanCode = MAKEWORD(scanCode, 0xe0);
        }
    } else {
        uint16_t vkCode = LOWORD(wparam);
        scanCode = LOWORD(this->u32->MapVirtualKeyW(vkCode, MAPVK_VK_TO_VSC_EX));
        if (scanCode == 0xe11d) {
            scanCode = 0xe046;
        }
    }
    index = LOBYTE(scanCode) | (HIBYTE(scanCode) ? 0x80 : 0x00);
    code = OMG_windows_scancode_table[index];
    return code;
}

void omg_window_win_check_dark_mode(OMG_WindowWin* this) {
    if (OMG_ISNOTNULL(this->uxtheme->ShouldAppsUseDarkMode) && (true || (omg_base->theme == OMG_THEME_NONE))) {
        omg_base->theme = this->uxtheme->ShouldAppsUseDarkMode() ? OMG_THEME_DARK : OMG_THEME_LIGHT;
    }
    if (OMG_ISNOTNULL(this->uxtheme->AllowDarkModeForWindow)) {
        this->uxtheme->AllowDarkModeForWindow(
            this->hwnd,
            ((omg_base->app_theme == OMG_THEME_DARK) || (omg_base->app_theme == OMG_THEME_AUTO)) && (omg_base->theme != OMG_THEME_LIGHT)
        );
    }
    if (OMG_ISNOTNULL(this->dwm->DwmSetWindowAttribute)) {
        DWORD val = (omg_base->theme == OMG_THEME_DARK) ? 1 : 0;
        this->dwm->DwmSetWindowAttribute(this->hwnd, 19, &val, sizeof(DWORD));
        val = (omg_base->theme == OMG_THEME_DARK) ? 1 : 0;
        this->dwm->DwmSetWindowAttribute(this->hwnd, 20, &val, sizeof(DWORD));
    }
}

bool omg_window_win_set_title(OMG_WindowWin* this, const OMG_String* new_title) {
    size_t count;
    _OMG_WIN_GET_ENCODE_SIZE(count, new_title, this->k32);
    if (count == 0)
        return true;
    wchar_t* out_buf = OMG_MALLOC(omg_base->mem, (size_t)count * 2 + 2);
    if (OMG_ISNULL(out_buf))
        return true;
    bool result = true;
    int out_len = this->k32->MultiByteToWideChar(CP_UTF8, 0, new_title->ptr, new_title->len, out_buf, (int)count);
    if (out_len > 0) {
        out_buf[out_len] = L'\0';
        result = !this->u32->SetWindowTextW(this->hwnd, out_buf);
    }
    OMG_FREE(omg_base->mem, out_buf);
    return result;
}

bool omg_window_win_set_state(OMG_WindowWin* this, int state) {
    if (state & OMG_WIN_STATE_MAXIMIZED)
        this->u32->ShowWindow(this->hwnd, SW_SHOWMAXIMIZED);
    if (state & OMG_WIN_STATE_MINIMIZED)
        this->u32->ShowWindow(this->hwnd, SW_MINIMIZE);
    if (state & OMG_WIN_STATE_RESTORED)
        this->u32->ShowWindow(this->hwnd, SW_RESTORE);
    this->u32->SetWindowPos(
        this->hwnd, NULL, 0, 0, 0, 0,
        SWP_NOZORDER | ((state & OMG_WIN_STATE_RESTORED) ? 0 : SWP_NOACTIVATE) | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED
    );
    return false;
}

bool omg_window_win_set_sys_button(OMG_WindowWin* this, int id, bool enabled) {
    int64_t style = GET_WIN_STYLE();
    if (!style)
        return true;
    int64_t index = (id & OMG_WIN_SYS_BUTTON_CLOSE) ? WS_SYSMENU : (
        (id & OMG_WIN_SYS_BUTTON_MAXIMIZE) ? WS_MAXIMIZEBOX : (
            (id & OMG_WIN_SYS_BUTTON_MINIMIZE) ? WS_MINIMIZEBOX : 0
        )
    );
    if (enabled) {
        base->sys_buttons |= id;
        style |= index;
    }
    else {
        base->sys_buttons &= ~id;
        style &= ~index;
    }
    // TODO: fuck microsoft because of close button
    return !SET_WIN_STYLE(style);
}

bool omg_window_win_set_resizable(OMG_WindowWin* this, bool enabled) {
    int64_t style = GET_WIN_STYLE();
    if (!style)
        return true;
    base->resizable = enabled;
    if (enabled)
        style |= WS_THICKFRAME;
    else
        style &= ~WS_THICKFRAME;
    bool res = !SET_WIN_STYLE(style);
    // IDK show this shit works, so let's fuck Microsoft again
    res = !this->u32->SetWindowPos(
        this->hwnd, NULL, 0, 0, 0, 0,
        SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED
    ) || res;
    return res;
}

bool omg_window_win_set_bordered(OMG_WindowWin* this, bool enabled) {
    int64_t style = GET_WIN_STYLE();
    if (!style)
        return true;
    base->bordered = enabled;
    if (enabled) {
        style &= ~WS_POPUP;
        style |= WS_CAPTION;
    }
    else {
        style |= WS_POPUP;
        style &= ~WS_CAPTION;
    }
    return !SET_WIN_STYLE(style);
}

bool omg_window_win_set_always_on_top(OMG_WindowWin* this, bool enabled) {
    int64_t style = GET_WIN_STYLE();
    if (!style)
        return true;
    base->always_on_top = enabled;
    // TODO
    return !SET_WIN_STYLE(style);
}

bool omg_window_win_renderer_alloc(OMG_WindowWin* this) {
    if (base->ren_type != OMG_REN_TYPE_SDL2)
        base->ren_type = OMG_REN_TYPE_AUTO;
    if (base->ren_type == OMG_REN_TYPE_AUTO) {
        base->ren_type = OMG_SUPPORT_SDL2 ? OMG_REN_TYPE_SDL2 : OMG_REN_TYPE_NONE;
        if (base->ren_type == OMG_REN_TYPE_NONE)
            return true;
        bool res = omg_window_win_renderer_alloc(this);
        if (res) {
            base->ren_type = OMG_REN_TYPE_AUTO;
        }
        return res;
    }
#if OMG_SUPPORT_SDL2
    if (base->ren_type == OMG_REN_TYPE_SDL2) {
        base->ren = OMG_MALLOC(omg_base->mem, sizeof(OMG_RendererSdl2) + sizeof(OMG_Sdl2));
        if (OMG_ISNULL(base->ren))
            return true;
        ren_sdl2->sdl2 = (void*)((size_t)base->ren + sizeof(OMG_RendererSdl2));
        if (omg_sdl2_dll_load(ren_sdl2->sdl2, omg_base->sdl2_dll_path)) {
            OMG_FREE(omg_base->mem, base->ren);
            base->ren = NULL;
            return true;
        }
        omg_renderer_fill_on_create(base->ren);
        ren_sdl2->id_cache[0] = -1;
        ren_sdl2->win_build_num = this->win_build_num;
        base->ren->was_allocated = true;
        base->ren->win = this;
        base->ren->omg = omg_base;
        OMG_BEGIN_POINTER_CAST();
        base->ren->init = omg_renderer_sdl2_init;
        base->ren->get_supported_drivers = omg_renderer_sdl2_get_supported_drivers;
        OMG_END_POINTER_CAST();
        if (ren_sdl2->sdl2->SDL_Init(SDL_INIT_VIDEO) < 0) {
            omg_sdl2_dll_free(ren_sdl2->sdl2);
            OMG_FREE(omg_base->mem, base->ren);
            base->ren = NULL;
            return true;
        }
        ren_sdl2->win = ren_sdl2->sdl2->SDL_CreateWindowFrom((const void*)this->hwnd);
        if (OMG_ISNULL(ren_sdl2->win)) {
            ren_sdl2->sdl2->SDL_Quit();
            omg_sdl2_dll_free(ren_sdl2->sdl2);
            OMG_FREE(omg_base->mem, base->ren);
            base->ren = NULL;
            return true;
        }
        return false;
    }
#endif
    return true;
}

bool omg_window_win_renderer_free(OMG_WindowWin* this) {
    if (OMG_ISNULL(base->ren))
        return true;
    bool res = base->ren->destroy(base->ren);
#if OMG_SUPPORT_SDL2
    if (base->ren->type == OMG_REN_TYPE_SDL2) {
        if (OMG_ISNOTNULL(base->ren)) {
            ren_sdl2->sdl2->SDL_DestroyWindow(ren_sdl2->win);
            ren_sdl2->sdl2->SDL_Quit();
            res = omg_sdl2_dll_free(ren_sdl2->sdl2) || res;
            ren_sdl2->sdl2 = NULL;
        }
    }
#endif
    omg_window_renderer_free((OMG_Window*)this);
    return res;
}

LRESULT omg_win_wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
#ifdef SetWindowLongPtrW
    OMG_WindowWin* this = (OMG_WindowWin*)OMG_WIN_CB_GetWindowLongW(hwnd, GWLP_USERDATA);
#else
    OMG_WindowWin* this = (OMG_WindowWin*)OMG_WIN_CB_GetWindowLongPtrW(hwnd, GWLP_USERDATA);
#endif
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            this->u32->BeginPaint(this->hwnd, &ps);
            OMG_EventPaint event;
            MAKE_EVENT(&event);
            event.win = this;
            omg_base->on_paint(&event);
            this->u32->EndPaint(this->hwnd, &ps);
            return RET_DEF_PROC();
        }
        case WM_MOUSEMOVE: {
            int x_pos = GET_X_LPARAM(lparam);
            int y_pos = GET_Y_LPARAM(lparam);
            if ((x_pos == this->mouse_pos_cache.x) && (y_pos == this->mouse_pos_cache.y))
                return FALSE;
            OMG_EventMouseMove event;
            MAKE_EVENT(&event);
            event.win = this;
            event.is_emulated = false;
            event.id = 0;
            event.pos.x = (float)x_pos;
            event.pos.y = (float)y_pos;
            event.rel.x = (float)(x_pos - this->mouse_pos_cache.x);
            event.rel.y = (float)(y_pos - this->mouse_pos_cache.y);
            MOUSE_FILL_STATE(event.state, wparam);
            omg_win_windows_check_mouse_buttons(this, wparam, 0);
            this->mouse_state_cache = event.state;
            this->last_mouse_state = wparam;
            if (this->is_mouse_left) {
                this->is_mouse_left = false;
                event.rel.x = event.rel.y = 0.0f;
                OMG_EventMouseFocus c_event;
                MAKE_EVENT(&c_event);
                c_event.win = this;
                c_event.state = event.state;
                c_event.pos.x = (float)event.pos.x;
                c_event.pos.y = (float)event.pos.y;
                c_event.is_focused = true;
                omg_base->on_mouse_enter(&c_event);
            }
            this->mouse_pos_cache.x = x_pos;
            this->mouse_pos_cache.y = y_pos;
            omg_base->on_mouse_move(&event);
            return FALSE;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            OMG_Scancode code = omg_window_win_code_to_scancode(this, lparam, wparam);
            if (code != OMG_SCANCODE_UNKNOWN) {
                OMG_EventKeyboard event;
                MAKE_EVENT(&event);
                event.win = this;
                event.is_pressed = (msg == WM_KEYDOWN) || (msg == WM_SYSKEYDOWN);
                if (!event.is_pressed && !omg_base->keyboard_state[code]) {
                    if (code == OMG_SCANCODE_PRINTSCREEN)
                        omg_win_wnd_proc(hwnd, WM_KEYDOWN, wparam, lparam); // Hack
                    else
                        return FALSE;
                }
                event.is_repeated = event.is_pressed && omg_base->keyboard_state[code];
                event.scancode = code;
                omg_base->keyboard_state[code] = event.is_pressed;
                event.sym = omg_keyboard_key_from_scancode(code);
                KEYBOARD_FILL_MOD(event.mod);
                (event.is_pressed ? omg_base->on_key_down : omg_base->on_key_up)(&event);
                if ((code == OMG_SCANCODE_F4) && (omg_base->keyboard_state[OMG_SCANCODE_LALT] || omg_base->keyboard_state[OMG_SCANCODE_RALT])) {
                    if (base->allow_alt_f4) {
                        if (!base->allow_alt) {
                            this->u32->SendMessageW(hwnd, WM_CLOSE, 0, 0);
                        }
                    }
                    else
                        return FALSE;
                }
            }
            return RET_DEF_PROC();
        }
        case WM_UNICHAR: {
            if (wparam == UNICODE_NOCHAR)
                return TRUE;
            char text[5];
            OMG_EventTextInput event;
            MAKE_EVENT(&event);
            event.win = this;
            OMG_String e_text = OMG_STRING_MAKE_BUFFER(text);
            if (omg_std_utf32_char_to_utf8((uint32_t)wparam, text))
                return FALSE;
            event.text = &e_text;
            omg_base->on_text_input(&event);
            return FALSE;
        }
        case WM_CHAR: {
            OMG_EventTextInput event;
            MAKE_EVENT(&event);
            event.win = this;
            if (IS_HIGH_SURROGATE(wparam)) {
                this->high_surrogate = (WCHAR)wparam;
            } else if (IS_SURROGATE_PAIR(this->high_surrogate, wparam)) {
                char text[5];
                if (!omg_std_win_utf16_char_to_utf8((UINT32)this->high_surrogate, (UINT32)wparam, text)) {
                    OMG_String e_text = OMG_STRING_MAKE_BUFFER(text);
                    event.text = &e_text;
                    omg_base->on_text_input(&event);
                }
                this->high_surrogate = 0;
            } else {
                char text[5];
                if (!omg_std_utf32_char_to_utf8((UINT32)wparam, text)) {
                    OMG_String e_text = OMG_STRING_MAKE_BUFFER(text);
                    event.text = &e_text;
                    omg_base->on_text_input(&event);
                }
            }
            return FALSE;
        }
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_XBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_XBUTTONUP: {
            OMG_EventMouseButton event;
            MAKE_EVENT(&event);
            event.is_emulated = false;
            event.is_pressed = (msg == WM_LBUTTONDOWN) || (msg == WM_MBUTTONDOWN) || (msg == WM_RBUTTONDOWN) || (msg == WM_XBUTTONDOWN);
            event.win = this;
            event.id = 0;
            if ((msg == WM_LBUTTONDOWN) || (msg == WM_LBUTTONUP))
                event.button = OMG_MBUTTON_LEFT;
            else if ((msg == WM_MBUTTONDOWN) || (msg == WM_MBUTTONUP))
                event.button = OMG_MBUTTON_MIDDLE;
            else if ((msg == WM_RBUTTONDOWN) || (msg == WM_RBUTTONUP))
                event.button = OMG_MBUTTON_RIGHT;
            else
                event.button = (GET_XBUTTON_WPARAM(wparam) == XBUTTON2) ? OMG_MBUTTON_X2 : OMG_MBUTTON_X1;
            event.pos.x = (float)GET_X_LPARAM(lparam);
            event.pos.y = (float)GET_Y_LPARAM(lparam);
            event.clicks = 1; // TODO
            if ((msg == WM_XBUTTONDOWN) || (msg == WM_XBUTTONUP)) {
                WORD need_wparam = GET_KEYSTATE_WPARAM(wparam);
                MOUSE_FILL_STATE(event.state, need_wparam);
                this->mouse_state_cache = event.state;
                this->last_mouse_state = wparam;
            }
            else {
                MOUSE_FILL_STATE(event.state, wparam);
                this->mouse_state_cache = event.state;
                this->last_mouse_state = wparam;
            }
            (event.is_pressed ? omg_base->on_mouse_down : omg_base->on_mouse_up)(&event);
            break;
        }
        case WM_POINTERUPDATE: {
            // TODO: check for touch, etc
            return RET_DEF_PROC();
        }
        case WM_ERASEBKGND: {
            return TRUE;
        }
        case WM_MOUSELEAVE: {
            this->is_mouse_left = true;
            OMG_EventMouseFocus event;
            MAKE_EVENT(&event);
            event.win = this;
            event.state = this->mouse_state_cache;
            event.pos.x = (float)this->mouse_pos_cache.x;
            event.pos.y = (float)this->mouse_pos_cache.y;
            event.is_focused = false;
            omg_base->on_mouse_leave(&event);
            return FALSE;
        }
        case WM_WINDOWPOSCHANGED: {
            WINDOWPOS* pos = (WINDOWPOS*)lparam;
            OMG_UNUSED(pos);
            return RET_DEF_PROC();
        }
        case WM_WINDOWPOSCHANGING: {
            WINDOWPOS* pos = (WINDOWPOS*)lparam;
            // Why??? Fuck Microsoft!
            if (!base->resizable && ((pos->cx != (int)this->size_cache.x) || (pos->cy != (int)this->size_cache.y))) {
                pos->cx = (int)this->size_cache.w;
                pos->cy = (int)this->size_cache.h;
                pos->flags |= SWP_NOSIZE;
                // return FALSE;
            }
            else if (base->resizable) {
                int min_size_x = (int)(base->min_size.w + this->size_cache.w - base->size.w);
                int min_size_y = (int)(base->min_size.h + this->size_cache.h - base->size.h);
                if (pos->cx < min_size_x)
                    pos->cx = min_size_x;
                if (pos->cy < min_size_y)
                    pos->cy = min_size_y;
                if (base->max_size.w > 0.0f) {
                    int max_size_x = (int)(base->max_size.w + this->size_cache.w - base->size.w);
                    if (pos->cx > max_size_x)
                        pos->cx = max_size_x;
                }
                if (base->max_size.h > 0.0f) {
                    int max_size_y = (int)(base->max_size.h + this->size_cache.h - base->size.h);
                    if (pos->cy > max_size_y)
                        pos->cy = max_size_y;
                }
                // return FALSE;
            }
            return RET_DEF_PROC();
        }
        case WM_GETMINMAXINFO: {
            if (OMG_ISNULL(this))
                return FALSE;
            // I think that WM_WINDOWPOSCHANGING is a way better
            return RET_DEF_PROC();
        }
        case WM_SIZING: {
            RECT* new_rect = (RECT*)lparam;
            if (!base->resizable) {
                new_rect->right = new_rect->left + (LONG)this->size_cache.w;
                new_rect->bottom = new_rect->top + (LONG)this->size_cache.h;
            }
            return TRUE;
        }
        case WM_SIZE: {
            UINT new_w = (UINT)LOWORD(lparam);
            UINT new_h = (UINT)HIWORD(lparam);
            if ((wparam == SIZE_MINIMIZED) && !(base->state & OMG_WIN_STATE_MINIMIZED)) {
                OMG_EventStateChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.change = OMG_WIN_STATE_MINIMIZED;
                event.prev_state = base->state;
                base->state &= ~OMG_WIN_STATE_RESTORED;
                base->state |= OMG_WIN_STATE_MINIMIZED;
                omg_base->on_state_change(&event);
            }
            if ((wparam == SIZE_MAXIMIZED) && !(base->state & OMG_WIN_STATE_MAXIMIZED)) {
                OMG_EventStateChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.change = OMG_WIN_STATE_MAXIMIZED;
                event.prev_state = base->state;
                base->state &= ~OMG_WIN_STATE_RESTORED;
                base->state |= OMG_WIN_STATE_MAXIMIZED;
                omg_base->on_state_change(&event);
            }
            if ((wparam != SIZE_MAXIMIZED) && (base->state & OMG_WIN_STATE_MAXIMIZED)) {
                OMG_EventStateChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.change = OMG_WIN_STATE_RESTORED;
                event.prev_state = base->state;
                base->state &= ~OMG_WIN_STATE_MAXIMIZED;
                base->state |= OMG_WIN_STATE_RESTORED;
                omg_base->on_state_change(&event);
            }
            if ((wparam != SIZE_MINIMIZED) && (base->state & OMG_WIN_STATE_MINIMIZED)) {
                OMG_EventStateChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.change = OMG_WIN_STATE_RESTORED;
                event.prev_state = base->state;
                base->state &= ~OMG_WIN_STATE_MINIMIZED;
                if (!(base->state & OMG_WIN_STATE_MAXIMIZED))
                    base->state |= OMG_WIN_STATE_RESTORED;
                omg_base->on_state_change(&event);
            }
            if (base->resizable && (new_w > 0) && (new_h > 0) && ((new_w != (UINT)base->size.w) || (new_h != (UINT)base->size.h))) {
                OMG_EventResize event;
                MAKE_EVENT(&event);
                event.win = this;
                base->size.w = event.size.w = (float)new_w;
                base->size.h = event.size.h = (float)new_h;
                RECT rect;
                RECT c_rect;
                if (this->u32->GetWindowRect(hwnd, &rect) && this->u32->GetClientRect(hwnd, &c_rect)) {
                    this->size_cache.w = base->size.w + (float)(rect.right - rect.left - c_rect.right);
                    this->size_cache.h = base->size.h + (float)(rect.bottom - rect.top - c_rect.bottom);
                }
                if (OMG_ISNOTNULL(base->ren))
                    base->ren->_on_update_window_size(base->ren);
                omg_base->on_size_change(&event);
                omg_base->on_resize(&event);
            }
            return RET_DEF_PROC();
        }
        case WM_SYSCOMMAND: {
            OMG_EventStateChanging event;
            MAKE_EVENT(&event);
            event.win = this;
            event.allow = true;
            if ((wparam == SC_VSCROLL) || (wparam == SC_HSCROLL) || ((wparam == SC_KEYMENU) && !base->allow_alt))
                return FALSE;
            else if (wparam == SC_CLOSE) {
                event.change = OMG_WIN_STATE_CLOSED;
                // in WM_CLOSE
                // omg_base->on_state_changing(&event);
                if (!event.allow)
                    return FALSE;
            }
            else if (wparam == SC_MAXIMIZE) {
                event.change = OMG_WIN_STATE_MAXIMIZED;
                omg_base->on_state_changing(&event);
                if (!event.allow)
                    return FALSE;
            }
            else if (wparam == SC_MINIMIZE) {
                event.change = OMG_WIN_STATE_MINIMIZED;
                omg_base->on_state_changing(&event);
                if (!event.allow)
                    return FALSE;
            }
            else if (wparam == SC_RESTORE) {
                event.change = OMG_WIN_STATE_RESTORED;
                omg_base->on_state_changing(&event);
                if (!event.allow)
                    return FALSE;
            }
            return RET_DEF_PROC();
        }
        case WM_THEMECHANGED: {
            omg_window_win_check_dark_mode(this);
            return RET_DEF_PROC();
        }
        case WM_NCCALCSIZE: {
            omg_window_win_update_scale(this);
            return RET_DEF_PROC();
        }
        case WM_MOUSEHWHEEL:
        case WM_MOUSEWHEEL: {
            short delta = GET_WHEEL_DELTA_WPARAM(wparam);
            if (delta == 0) // Am I right?
                return FALSE;
            WORD param_state = GET_KEYSTATE_WPARAM(wparam);
            OMG_EventMouseWheel event;
            MAKE_EVENT(&event);
            event.win = this;
            event.id = 0;
            event.is_emulated = false;
            MOUSE_FILL_STATE(event.state, param_state);
            omg_win_windows_check_mouse_buttons(this, (WPARAM)param_state, 0);
            this->mouse_state_cache = event.state;
            this->last_mouse_state = wparam;
            if (msg == WM_MOUSEWHEEL) {
                event.rel.x = 0.0f;
                event.rel.y = (float)(-delta) / (float)WHEEL_DELTA;
            }
            else {
                event.rel.x = (float)delta / (float)WHEEL_DELTA;
                event.rel.y = 0.0f;
            }
            event.mouse_pos.x = (float)GET_X_LPARAM(lparam);
            event.mouse_pos.y = (float)GET_Y_LPARAM(lparam);
            omg_base->on_mouse_wheel(&event);
            return FALSE;
        }
        case WM_NCACTIVATE:
        case WM_ACTIVATE:
        case WM_ENTERIDLE:
        case WM_SETFOCUS:
        case WM_KILLFOCUS: {
            bool is_focused = ((msg == WM_SETFOCUS) || (msg == WM_KILLFOCUS)) ?
                (msg == WM_SETFOCUS) :
                (this->u32->GetForegroundWindow() == hwnd);
            if (is_focused != this->is_focused) {
                this->is_focused = is_focused;
                OMG_EventFocusChange event;
                MAKE_EVENT(&event);
                event.win = this;
                event.is_focused = is_focused;
                omg_base->on_focus_change(&event);
            }
            return RET_DEF_PROC();
        }
        case WM_INPUTLANGCHANGE: {
            // TODO: Keymap Update
            return RET_DEF_PROC();
        }
        case WM_CLOSE: {
            OMG_EventStateChanging event;
            MAKE_EVENT(&event);
            event.win = this;
            event.allow = true;
            event.change = OMG_WIN_STATE_CLOSED;
            omg_base->on_state_changing(&event);
            if (event.allow) {
                base->enable_paint = false;
                return RET_DEF_PROC();
            }
            return FALSE;
        }
        case WM_DESTROY: {
            this->destroyed = true;
            OMG_EventClose event;
            MAKE_EVENT(&event);
            event.win = this;
            omg_base->on_close(&event); // Maybe in WM_CLOSE?
            this->u32->PostQuitMessage(0);
            return FALSE;
        }
        case WM_NCCREATE: {
            LPCREATESTRUCTW lps = (LPCREATESTRUCTW)lparam;
            this = (OMG_WindowWin*)lps->lpCreateParams;
            if (OMG_ISNULL(this->u32->SetWindowLongPtrW))
                this->u32->SetWindowLongW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
            else
                this->u32->SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);
            if (omg_base->support_highdpi && OMG_ISNOTNULL(this->u32->EnableNonClientDpiScaling))
                this->u32->EnableNonClientDpiScaling(hwnd);
            return RET_DEF_PROC();
        }
        default: {
            if (OMG_ISNOTNULL(this)) {
                // _OMG_LOG_WARN(omg_base, "TODO Event: ", (int)msg);
                return RET_DEF_PROC();
            }
            return 0;
        }
    }
    if (OMG_ISNOTNULL(this))
        return RET_DEF_PROC();
    else
        return 0;
}

bool omg_window_win_destroy(OMG_WindowWin* this) {
    bool result = false;
    if (base->inited) {
        omg_window_destroy((OMG_Window*)this);
        if (!this->destroyed && OMG_ISNOTNULL(this->hwnd) && !this->u32->DestroyWindow(this->hwnd)) {
            _OMG_LOG_WARN(omg_base, "Failed to destroy window");
            result = true;
        }
        this->destroyed = true;
        if (!this->u32->UnregisterClassW(this->wc.lpszClassName, this->wc.hInstance)) {
            _OMG_LOG_WARN(omg_base, "Failed to unregister class");
            result = true;
        }
        base->inited = false;
    }
    return result;
}

void omg_window_win_update_scale(OMG_WindowWin* this) {
    OMG_FPoint new_scale;
    if (omg_base->support_highdpi) {
        if (OMG_ISNULL(this->u32->GetDpiForWindow)) {
            HDC hdc = this->u32->GetWindowDC(this->hwnd);
            new_scale.x = (float)this->g32->GetDeviceCaps(hdc, LOGPIXELSX) / 96.0f;
            new_scale.y = (float)this->g32->GetDeviceCaps(hdc, LOGPIXELSY) / 96.0f;
            this->u32->ReleaseDC(this->hwnd, hdc);
        }
        else {
            new_scale.x = new_scale.y = (float)this->u32->GetDpiForWindow(this->hwnd) / 96.0f;
        }
    }
    else {
        new_scale.x = new_scale.y = 1.0f;
    }
    if (new_scale.x == base->scale.x && new_scale.y == base->scale.y)
        return;
    if (base->scale.x == 0.0f)
        base->scale.x = 1.0f;
    RECT desktop_rect;
    RECT rect;
    RECT c_rect;
    HWND hwnd_d = this->u32->GetDesktopWindow();
    if (!this->u32->GetClientRect(hwnd_d, &desktop_rect) || !this->u32->GetWindowRect(this->hwnd, &rect) || !this->u32->GetClientRect(this->hwnd, &c_rect))
        return;
    this->size_cache.w = base->size.w/* * new_scale.x*/ + (float)(rect.right - rect.left - c_rect.right);
    this->size_cache.h = base->size.h/* * new_scale.y*/ + (float)(rect.bottom - rect.top - c_rect.bottom);
    this->u32->SetWindowPos(
        this->hwnd,
        NULL,
        (int)(((float)desktop_rect.right - this->size_cache.w) / 2.0f),
        (int)(((float)desktop_rect.bottom - this->size_cache.h) / 2.0f),
        (int)this->size_cache.w, (int)this->size_cache.h,
        SWP_NOZORDER | SWP_NOACTIVATE | (base->centered ? 0 : SWP_NOMOVE) | SWP_FRAMECHANGED
    );
}

bool omg_window_win_init(OMG_WindowWin* this) {
    omg_window_init(base);
    base->type = OMG_WIN_TYPE_WIN;
    base->inited = false;
    this->destroyed = false;
    this->is_mouse_left = true;
    this->is_focused = false;
    this->high_surrogate = 0;
    this->mouse_state_cache = 0;
    this->last_mouse_state = 0;
    this->mouse_pos_cache.x = this->mouse_pos_cache.y = 0;
    this->wc.cbSize = sizeof(WNDCLASSEXW);
    this->wc.style = CS_HREDRAW | CS_VREDRAW;
    this->wc.lpfnWndProc = (WNDPROC)omg_win_wnd_proc;
    this->wc.cbClsExtra = 0;
    this->wc.cbWndExtra = 0;
#if _OMG_ENTRY_DATA_TYPE == 2
    this->wc.hInstance = omg_base->entry_data->hInst;
#else
    this->wc.hInstance = (HINSTANCE)this->k32->GetModuleHandleW(NULL);
#endif
    this->wc.hIcon = NULL;
    this->wc.hCursor = (HCURSOR)this->u32->LoadImageW(
        NULL, (LPCWSTR)IDC_ARROW,
        IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED
    );
    this->wc.hbrBackground = this->u32->GetSysColorBrush(COLOR_3DFACE);
    this->wc.lpszMenuName = NULL;
    this->wc.lpszClassName = L"OMG_App"; // TODO: custom name to support multiple windows
    this->wc.hIconSm = NULL;
    if (OMG_ISNOTNULL(this->u32->GetWindowLongPtrW))
        OMG_WIN_CB_GetWindowLongPtrW = this->u32->GetWindowLongPtrW;
    if (OMG_ISNOTNULL(this->u32->GetWindowLongW))
        OMG_WIN_CB_GetWindowLongW = this->u32->GetWindowLongW;
    this->class_atom = this->u32->RegisterClassExW(&this->wc);
    if (!this->class_atom) {
        DWORD error = this->k32->GetLastError();
        DWORD res;
        wchar_t* w_error_buffer;
        _OMG_WIN_FORMAT_ERROR(omg_base->mem, this->k32, error, w_error_buffer, res);
        if (res > 2) {
            w_error_buffer[res - 3] = L'\0';
            _OMG_LOG_ERROR(omg_base, "Failed to register class atom (", w_error_buffer, ")");
        }
        else
            _OMG_LOG_ERROR(omg_base, "Failed to register class atom (Error Code - ", error, ")");
        if (OMG_ISNOTNULL(w_error_buffer))
            OMG_FREE(omg_base->mem, w_error_buffer);
        return true;
    }
    this->hwnd = this->u32->CreateWindowExW(
        WS_EX_COMPOSITED | WS_EX_LAYERED | WS_EX_NOINHERITLAYOUT,
        this->wc.lpszClassName, L"OMG Window [Win32]",
        (base->bordered ? WS_CAPTION : WS_POPUP) |
        ((base->sys_buttons & OMG_WIN_SYS_BUTTON_CLOSE) ? WS_SYSMENU : 0) | \
        ((base->sys_buttons & OMG_WIN_SYS_BUTTON_MINIMIZE) ? WS_MINIMIZEBOX : 0) | \
        ((base->sys_buttons & OMG_WIN_SYS_BUTTON_MAXIMIZE) ? WS_MAXIMIZEBOX : 0) | \
        ((base->state & OMG_WIN_STATE_MAXIMIZED) ? WS_MAXIMIZE : 0) | \
        ((base->state & OMG_WIN_STATE_MINIMIZED) ? WS_MINIMIZE : 0) | \
        (base->resizable ? WS_THICKFRAME : 0) | \
        WS_OVERLAPPED,
        CW_USEDEFAULT, CW_USEDEFAULT,
        (int)base->size.w + 50, (int)base->size.h + 50,
        NULL, NULL, this->wc.hInstance, this
    );
    if (OMG_ISNULL(this->hwnd)) {
        DWORD error = this->k32->GetLastError();
        DWORD res;
        wchar_t* w_error_buffer;
        _OMG_WIN_FORMAT_ERROR(omg_base->mem, this->k32, error, w_error_buffer, res);
        if (res > 2) {
            w_error_buffer[res - 3] = L'\0';
            _OMG_LOG_ERROR(omg_base, "Failed to create window (", w_error_buffer, ")");
        }
        else
            _OMG_LOG_ERROR(omg_base, "Failed to create window (Error Code - ", error, ")");
        if (OMG_ISNOTNULL(w_error_buffer))
            OMG_FREE(omg_base->mem, w_error_buffer);
        this->u32->UnregisterClassW(this->wc.lpszClassName, this->wc.hInstance);
        return true;
    }
    omg_window_win_check_dark_mode(this);
    base->scale.x = 0.0f; // Hack
    omg_window_win_update_scale(this);
    OMG_BEGIN_POINTER_CAST();
    base->set_state = omg_window_win_set_state;
    base->set_sys_button = omg_window_win_set_sys_button;
    base->set_resizable = omg_window_win_set_resizable;
    base->set_bordered = omg_window_win_set_bordered;
    base->set_always_on_top = omg_window_win_set_always_on_top;
    base->show = omg_window_win_show;
    base->set_title = omg_window_win_set_title;
    base->destroy = omg_window_win_destroy;
    base->renderer_alloc = omg_window_win_renderer_alloc;
    base->renderer_free = omg_window_win_renderer_free;
    OMG_END_POINTER_CAST();
    base->inited = true;
    _OMG_LOG_INFO(omg_base, "Win32 window created successfuly");
    return false;
}
#endif
