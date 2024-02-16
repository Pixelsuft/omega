#include <omega/omega_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/memory_raylib.h>
#include <omega/winmgr_raylib.h>
#include <omega/window_raylib.h>
#include <omega/clock_raylib.h>
#include <omega/audio_raylib.h>
#include <omega/audio_fmod.h>
#define base ((OMG_Omega*)this)
#define audio_raylib ((OMG_AudioRaylib*)base->audio)
#define audio_fmod ((OMG_AudioFmod*)base->audio)
#define winmgr_raylib ((OMG_WinmgrRaylib*)base->winmgr)
#define MOUSE_FILL_STATE(state) do { \
    state = 0; \
    if (this->raylib->IsMouseButtonDown(MOUSE_BUTTON_LEFT)) \
        state |= OMG_MBUTTON_LMASK; \
    if (this->raylib->IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) \
        state |= OMG_MBUTTON_MMASK; \
    if (this->raylib->IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) \
        state |= OMG_MBUTTON_RMASK; \
    if (this->raylib->IsMouseButtonDown(MOUSE_BUTTON_SIDE)) \
        state |= OMG_MBUTTON_X1MASK; \
    if (this->raylib->IsMouseButtonDown(MOUSE_BUTTON_EXTRA)) \
        state |= OMG_MBUTTON_X2MASK; \
    if (this->raylib->IsMouseButtonDown(MOUSE_BUTTON_FORWARD)) \
        state |= OMG_MBUTTON_X3MASK; \
    if (this->raylib->IsMouseButtonDown(MOUSE_BUTTON_BACK)) \
        state |= OMG_MBUTTON_X4MASK; \
} while (0)
#define KEYBOARD_FILL_MOD(mod) do { \
    mod = 0; \
    if (this->raylib->IsKeyDown(KEY_LEFT_SHIFT)) \
        mod |= OMG_KMOD_LSHIFT; \
    if (this->raylib->IsKeyDown(KEY_RIGHT_SHIFT)) \
        mod |= OMG_KMOD_RSHIFT; \
    if (this->raylib->IsKeyDown(KEY_LEFT_CONTROL)) \
        mod |= OMG_KMOD_LCTRL; \
    if (this->raylib->IsKeyDown(KEY_RIGHT_CONTROL)) \
        mod |= OMG_KMOD_RCTRL; \
    if (this->raylib->IsKeyDown(KEY_LEFT_ALT)) \
        mod |= OMG_KMOD_LALT; \
    if (this->raylib->IsKeyDown(KEY_RIGHT_ALT)) \
        mod |= OMG_KMOD_RALT; \
    if (this->raylib->IsKeyDown(KEY_LEFT_SUPER)) \
        mod |= OMG_KMOD_LGUI; \
    if (this->raylib->IsKeyDown(KEY_RIGHT_SUPER)) \
        mod |= OMG_KMOD_RGUI; \
} while (0)
#define MAKE_EVENT(event) do { \
    ((OMG_Event*)event)->omg = this; \
    ((OMG_Event*)event)->data = base->event_arg; \
    ((OMG_Event*)event)->time = 0; \
} while (0)

void omg_raylib_fill_after_create(OMG_OmegaRaylib* this, OMG_EntryData* data) {
    this->raylib = NULL;
    OMG_BEGIN_POINTER_CAST();
    omg_fill_on_create(this, data);
    base->omg_init = omg_raylib_init;
    OMG_END_POINTER_CAST();
}

OMG_OmegaRaylib* omg_raylib_create(OMG_EntryData* data) {
    OMG_OmegaRaylib* result = (OMG_OmegaRaylib*)omg_alloc_omega_internal(data, sizeof(OMG_OmegaRaylib));
    omg_raylib_fill_after_create(result, data);
    return result;
}

void omg_raylib_fill_std(OMG_OmegaRaylib* this) {
    base->std->memory_allocator = base->mem;
}

bool omg_raylib_log_str_type(OMG_OmegaRaylib* this, const OMG_String* data, int type) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    int rl_type = 0;
    if (type == OMG_LOG_CATEGORY_INFO)
        rl_type = LOG_INFO;
    else if (type == OMG_LOG_CATEGORY_WARN)
        rl_type = LOG_WARNING;
    else if (type == OMG_LOG_CATEGORY_ERROR)
        rl_type = LOG_ERROR;
    else
        rl_type = LOG_FATAL;
    this->raylib->TraceLog(rl_type, data->ptr);
    return false;
}

bool omg_raylib_alloc_winmgr(OMG_OmegaRaylib* this) {
    base->winmgr = OMG_MALLOC(base->mem, sizeof(OMG_WinmgrRaylib));
    if (OMG_ISNULL(base->winmgr))
        return true;
    omg_alloc_winmgr((OMG_Omega*)this);
    winmgr_raylib->raylib = this->raylib;
    OMG_BEGIN_POINTER_CAST();
    base->winmgr->init = omg_winmgr_raylib_init;
    OMG_END_POINTER_CAST();
    return false;
}

void omg_raylib_poll_events(OMG_OmegaRaylib* this) {
    OMG_Window* win = base->winmgr->cache[0];
    if (OMG_ISNULL(win))
        return;
    if (this->raylib->WindowShouldClose()) {
        OMG_EventStateChanging c_event;
        MAKE_EVENT(&c_event);
        c_event.win = win;
        c_event.allow = true;
        c_event.change = OMG_WIN_STATE_CLOSED;
        base->on_state_changing(&c_event);
        if (c_event.allow) {
            OMG_EventClose close_event;
            MAKE_EVENT(&close_event);
            close_event.win = win;
            base->on_close(&close_event);
            OMG_EventQuit event;
            MAKE_EVENT(&event);
            base->on_quit(&event);
        }
    }
    if (this->raylib->IsWindowResized()) {
        float new_w = (float)this->raylib->GetScreenWidth();
        float new_h = (float)this->raylib->GetScreenHeight();
        bool should_limit = false;
        if (new_w < win->min_size.w) {
            should_limit = true;
            new_w = win->min_size.w;
        }
        else if ((win->max_size.w > 0.0f) && (new_w > win->max_size.w)) {
            should_limit = true;
            new_w = win->max_size.w;
        }
        if (new_h < win->min_size.h) {
            should_limit = true;
            new_h = win->min_size.h;
        }
        else if ((win->max_size.h > 0.0f) && (new_h > win->max_size.h)) {
            should_limit = true;
            new_h = win->max_size.h;
        }
        if (should_limit)
            this->raylib->SetWindowSize((int)new_w, (int)new_h);
        OMG_EventResize event;
        MAKE_EVENT(&event);
        event.win = win;
        win->size.w = event.size.w = (float)this->raylib->GetScreenWidth();
        win->size.h = event.size.h = (float)this->raylib->GetScreenHeight();
        if (OMG_ISNOTNULL(win->ren))
            win->ren->_on_update_window_size(win->ren);
        base->on_size_change(&event);
        base->on_resize(&event);
    }
    bool is_max = this->raylib->IsWindowMaximized();
    bool is_min = this->raylib->IsWindowMinimized();
    if (is_min && !(win->state & OMG_WIN_STATE_MINIMIZED)) {
        OMG_EventStateChanging c_event;
        MAKE_EVENT(&c_event);
        c_event.win = win;
        c_event.allow = true;
        c_event.change = OMG_WIN_STATE_MINIMIZED;
        base->on_state_changing(&c_event);
        if (c_event.allow) {
            OMG_EventStateChange event;
            MAKE_EVENT(&event);
            event.win = win;
            event.change = OMG_WIN_STATE_MINIMIZED;
            event.prev_state = win->state;
            win->state &= ~OMG_WIN_STATE_RESTORED;
            win->state |= OMG_WIN_STATE_MINIMIZED;
            base->on_state_change(&event);
        }
        else
            this->raylib->RestoreWindow();
    }
    if (is_max && !(win->state & OMG_WIN_STATE_MAXIMIZED)) {
        OMG_EventStateChanging c_event;
        MAKE_EVENT(&c_event);
        c_event.win = win;
        c_event.allow = true;
        c_event.change = OMG_WIN_STATE_MAXIMIZED;
        base->on_state_changing(&c_event);
        if (c_event.allow) {
            OMG_EventStateChange event;
            MAKE_EVENT(&event);
            event.win = win;
            event.change = OMG_WIN_STATE_MAXIMIZED;
            event.prev_state = win->state;
            win->state &= ~OMG_WIN_STATE_RESTORED;
            win->state |= OMG_WIN_STATE_MAXIMIZED;
            base->on_state_change(&event);
        }
        else
            this->raylib->RestoreWindow();
    }
    if (!is_max && (win->state & OMG_WIN_STATE_MAXIMIZED)) {
        OMG_EventStateChanging c_event;
        MAKE_EVENT(&c_event);
        c_event.win = win;
        c_event.allow = true;
        c_event.change = OMG_WIN_STATE_RESTORED;
        base->on_state_changing(&c_event);
        if (c_event.allow) {
            OMG_EventStateChange event;
            MAKE_EVENT(&event);
            event.win = win;
            event.change = OMG_WIN_STATE_RESTORED;
            event.prev_state = win->state;
            win->state &= ~OMG_WIN_STATE_MAXIMIZED;
            win->state |= OMG_WIN_STATE_RESTORED;
            base->on_state_change(&event);
        }
        else
            this->raylib->MaximizeWindow();
    }
    if (!is_min && (win->state & OMG_WIN_STATE_MINIMIZED)) {
        OMG_EventStateChanging c_event;
        MAKE_EVENT(&c_event);
        c_event.win = win;
        c_event.allow = true;
        c_event.change = OMG_WIN_STATE_RESTORED;
        base->on_state_changing(&c_event);
        if (c_event.allow) {
            OMG_EventStateChange event;
            MAKE_EVENT(&event);
            event.win = win;
            event.change = (is_max ? OMG_WIN_STATE_MAXIMIZED : OMG_WIN_STATE_RESTORED);
            event.prev_state = win->state;
            win->state &= ~OMG_WIN_STATE_MINIMIZED;
            win->state |= (is_max ? OMG_WIN_STATE_MAXIMIZED : OMG_WIN_STATE_RESTORED);
            base->on_state_change(&event);
        }
        else
            this->raylib->MinimizeWindow();
    }
    if (this->raylib->IsWindowFocused()) {
        if (!this->is_focused) {
            this->is_focused = true;
            OMG_EventFocusChange event;
            MAKE_EVENT(&event);
            event.win = win;
            event.is_focused = true;
            base->on_focus_change(&event);
        }
    }
    else if (this->is_focused) {
        this->is_focused = false;
        OMG_EventFocusChange event;
        MAKE_EVENT(&event);
        event.win = win;
        event.is_focused = false;
        base->on_focus_change(&event);
    }
    Vector2 mouse_delta = this->raylib->GetMouseDelta();
    Vector2 mouse_pos = this->raylib->GetMousePosition();
    if ((mouse_delta.x != 0.0f) || (mouse_delta.y != 0.0f)) {
        OMG_EventMouseMove event;
        MAKE_EVENT(&event);
        event.is_emulated = false;
        event.win = win;
        event.id = 0;
        MOUSE_FILL_STATE(event.state);
        base->mouse_state = event.state;
        event.pos.x = mouse_pos.x;
        event.pos.y = mouse_pos.y;
        event.rel.x = mouse_delta.x;
        event.rel.y = mouse_delta.y;
        base->on_mouse_move(&event);
    }
    Vector2 wheel_delta = this->raylib->GetMouseWheelMoveV();
    if ((wheel_delta.x != 0.0f) || (wheel_delta.y != 0.0f)) {
        OMG_EventMouseWheel event;
        MAKE_EVENT(&event);
        event.is_emulated = false;
        event.win = win;
        event.id = 0;
        MOUSE_FILL_STATE(event.state);
        base->mouse_state = event.state;
        event.rel.x = wheel_delta.x;
        event.rel.y = -wheel_delta.y;
        event.mouse_pos.x = mouse_pos.x;
        event.mouse_pos.y = mouse_pos.y;
        base->on_mouse_wheel(&event);
    }
    for (int i = 0; i < 7; i++) {
        bool pressed = this->raylib->IsMouseButtonPressed(i);
        if (pressed || this->raylib->IsMouseButtonReleased(i)) {
            OMG_EventMouseButton event;
            MAKE_EVENT(&event);
            event.is_emulated = false;
            event.win = win;
            event.id = 0;
            event.button = i + 1;
            if (event.button == 2)
                event.button = 3;
            else if (event.button == 3)
                event.button = 2;
            event.pos.x = mouse_pos.x;
            event.pos.y = mouse_pos.y;
            event.clicks = 1; // TODO
            MOUSE_FILL_STATE(event.state);
            base->mouse_state = event.state;
            event.is_pressed = pressed;
            (pressed ? base->on_mouse_down : base->on_mouse_up)(&event);
        }
    }
    bool is_pressed;
    bool is_repeated;
    bool is_released;
    for (int i = 0; i < 350; i++) {
        uint32_t code = this->scancode_map[i];
        if (code == 0)
            continue;
        is_pressed = this->raylib->IsKeyPressed(i);
        is_repeated = this->raylib->IsKeyPressedRepeat(i);
        is_released = this->raylib->IsKeyReleased(i);
        if (is_pressed || is_repeated || is_released) {
            OMG_EventKeyboard event;
            MAKE_EVENT(&event);
            event.win = win;
            event.is_pressed = !is_released;
            event.is_repeated = event.is_pressed && is_repeated;
            event.code = code;
            event.sym = omg_keyboard_key_from_scancode(code);
            base->keyboard_state[code] = event.is_pressed;
            KEYBOARD_FILL_MOD(event.mod);
            (event.is_pressed ? base->on_key_down : base->on_key_up)(&event);
        }
    }
    int key_pressed = this->raylib->GetCharPressed();
    while (key_pressed) {
        char buf[5];
        omg_std_unicode_char_to_utf8(buf, (int32_t)key_pressed);
        OMG_String text = OMG_STRING_MAKE_BUFFER(buf);
        OMG_EventTextInput event;
        MAKE_EVENT(&event);
        event.win = win;
        event.text = &text;
        base->on_text_input(&event);
        key_pressed = this->raylib->GetCharPressed();
    }
}

void omg_raylib_auto_loop_run(OMG_OmegaRaylib* this) {
    base->looping = true;
    OMG_EventUpdate u_event;
    OMG_EventPaint p_event;
    while (base->looping) {
        omg_raylib_poll_events(this);
        if (!base->looping)
            break;
        MAKE_EVENT(&u_event);
        base->on_update(&u_event);
        if (base->enable_paint && OMG_ISNOTNULL(base->winmgr->cache[0]) && base->winmgr->cache[0]->enable_paint) {
            MAKE_EVENT(&p_event);
            p_event.win = base->winmgr->cache[0];
            base->on_paint(&p_event);
        }
    }
    OMG_EventLoopStop ls_event;
    MAKE_EVENT(&ls_event);
    base->on_loop_stop(&ls_event);
}

bool omg_raylib_app_init(OMG_OmegaRaylib* this) {
    this->is_focused = false;
    base->keyboard_state = OMG_MALLOC(base->mem, (OMG_NUM_SCANCODES * (sizeof(bool) + sizeof(OMG_Keycode))) + (350 * sizeof(uint32_t)));
    if (OMG_ISNULL(base->keyboard_state))
        return true;
    base->keymap = (const OMG_Keycode*)((size_t)base->keyboard_state + OMG_NUM_SCANCODES * sizeof(bool));
    base->std->memset(base->keyboard_state, 0, OMG_NUM_SCANCODES * sizeof(bool));
    omg_keyboard_init_keymap(base);
    this->scancode_map = (uint32_t*)((size_t)base->keymap + ((size_t)OMG_NUM_SCANCODES * sizeof(OMG_Keycode)));
    base->std->memset(this->scancode_map, 0, 350 * sizeof(uint32_t));
    this->scancode_map[KEY_NULL] = 0;
    this->scancode_map[KEY_APOSTROPHE] = OMG_SCANCODE_APOSTROPHE;
    this->scancode_map[KEY_COMMA] = OMG_SCANCODE_COMMA;
    this->scancode_map[KEY_MINUS] = OMG_SCANCODE_MINUS;
    this->scancode_map[KEY_PERIOD] = OMG_SCANCODE_PERIOD;
    this->scancode_map[KEY_SLASH] = OMG_SCANCODE_SLASH;
    this->scancode_map[KEY_ZERO] = OMG_SCANCODE_0;
    this->scancode_map[KEY_ONE] = OMG_SCANCODE_1;
    this->scancode_map[KEY_TWO] = OMG_SCANCODE_2;
    this->scancode_map[KEY_THREE] = OMG_SCANCODE_3;
    this->scancode_map[KEY_FOUR] = OMG_SCANCODE_4;
    this->scancode_map[KEY_FIVE] = OMG_SCANCODE_5;
    this->scancode_map[KEY_SIX] = OMG_SCANCODE_6;
    this->scancode_map[KEY_SEVEN] = OMG_SCANCODE_7;
    this->scancode_map[KEY_EIGHT] = OMG_SCANCODE_8;
    this->scancode_map[KEY_NINE] = OMG_SCANCODE_9;
    this->scancode_map[KEY_SEMICOLON] = OMG_SCANCODE_SEMICOLON;
    this->scancode_map[KEY_EQUAL] = OMG_SCANCODE_EQUALS;
    this->scancode_map[KEY_LEFT_BRACKET] = OMG_SCANCODE_LEFTBRACKET;
    this->scancode_map[KEY_BACKSLASH] = OMG_SCANCODE_BACKSLASH;
    this->scancode_map[KEY_RIGHT_BRACKET] = OMG_SCANCODE_RIGHTBRACKET;
    this->scancode_map[KEY_GRAVE] = OMG_SCANCODE_GRAVE;
    this->scancode_map[KEY_SPACE] = OMG_SCANCODE_SPACE;
    this->scancode_map[KEY_ESCAPE] = OMG_SCANCODE_ESCAPE;
    this->scancode_map[KEY_ENTER] = OMG_SCANCODE_RETURN;
    this->scancode_map[KEY_TAB] = OMG_SCANCODE_TAB;
    this->scancode_map[KEY_BACKSPACE] = OMG_SCANCODE_BACKSPACE;
    this->scancode_map[KEY_INSERT] = OMG_SCANCODE_INSERT;
    this->scancode_map[KEY_DELETE] = OMG_SCANCODE_DELETE;
    this->scancode_map[KEY_RIGHT] = OMG_SCANCODE_RIGHT;
    this->scancode_map[KEY_LEFT] = OMG_SCANCODE_LEFT;
    this->scancode_map[KEY_DOWN] = OMG_SCANCODE_DOWN;
    this->scancode_map[KEY_UP] = OMG_SCANCODE_UP;
    this->scancode_map[KEY_PAGE_UP] = OMG_SCANCODE_PAGEUP;
    this->scancode_map[KEY_PAGE_DOWN] = OMG_SCANCODE_PAGEDOWN;
    this->scancode_map[KEY_HOME] = OMG_SCANCODE_HOME;
    this->scancode_map[KEY_END] = OMG_SCANCODE_END;
    this->scancode_map[KEY_CAPS_LOCK] = OMG_SCANCODE_CAPSLOCK;
    this->scancode_map[KEY_SCROLL_LOCK] = OMG_SCANCODE_SCROLLLOCK;
    this->scancode_map[KEY_NUM_LOCK] = OMG_SCANCODE_NUMLOCKCLEAR;
    this->scancode_map[KEY_PRINT_SCREEN] = OMG_SCANCODE_PRINTSCREEN;
    this->scancode_map[KEY_PAUSE] = OMG_SCANCODE_PAUSE;
    this->scancode_map[KEY_F1] = OMG_SCANCODE_F1;
    this->scancode_map[KEY_F2] = OMG_SCANCODE_F2;
    this->scancode_map[KEY_F3] = OMG_SCANCODE_F3;
    this->scancode_map[KEY_F4] = OMG_SCANCODE_F4;
    this->scancode_map[KEY_F5] = OMG_SCANCODE_F5;
    this->scancode_map[KEY_F6] = OMG_SCANCODE_F6;
    this->scancode_map[KEY_F7] = OMG_SCANCODE_F7;
    this->scancode_map[KEY_F8] = OMG_SCANCODE_F8;
    this->scancode_map[KEY_F9] = OMG_SCANCODE_F9;
    this->scancode_map[KEY_F10] = OMG_SCANCODE_F10;
    this->scancode_map[KEY_F11] = OMG_SCANCODE_F11;
    this->scancode_map[KEY_F12] = OMG_SCANCODE_F12;
    this->scancode_map[KEY_LEFT_SHIFT] = OMG_SCANCODE_LSHIFT;
    this->scancode_map[KEY_LEFT_CONTROL] = OMG_SCANCODE_LCTRL;
    this->scancode_map[KEY_LEFT_ALT] = OMG_SCANCODE_LALT;
    this->scancode_map[KEY_LEFT_SUPER] = OMG_SCANCODE_LGUI;
    this->scancode_map[KEY_RIGHT_SHIFT] = OMG_SCANCODE_RSHIFT;
    this->scancode_map[KEY_RIGHT_CONTROL] = OMG_SCANCODE_RCTRL;
    this->scancode_map[KEY_RIGHT_ALT] = OMG_SCANCODE_RALT;
    this->scancode_map[KEY_RIGHT_SUPER] = OMG_SCANCODE_RGUI;
    this->scancode_map[KEY_KB_MENU] = OMG_SCANCODE_KBDILLUMTOGGLE;
    this->scancode_map[KEY_KP_0] = OMG_SCANCODE_KP_0;
    this->scancode_map[KEY_KP_1] = OMG_SCANCODE_KP_1;
    this->scancode_map[KEY_KP_2] = OMG_SCANCODE_KP_2;
    this->scancode_map[KEY_KP_3] = OMG_SCANCODE_KP_3;
    this->scancode_map[KEY_KP_4] = OMG_SCANCODE_KP_4;
    this->scancode_map[KEY_KP_5] = OMG_SCANCODE_KP_5;
    this->scancode_map[KEY_KP_6] = OMG_SCANCODE_KP_6;
    this->scancode_map[KEY_KP_7] = OMG_SCANCODE_KP_7;
    this->scancode_map[KEY_KP_8] = OMG_SCANCODE_KP_8;
    this->scancode_map[KEY_KP_9] = OMG_SCANCODE_KP_9;
    this->scancode_map[KEY_KP_DECIMAL] = OMG_SCANCODE_KP_DECIMAL;
    this->scancode_map[KEY_KP_DIVIDE] = OMG_SCANCODE_KP_DIVIDE;
    this->scancode_map[KEY_KP_MULTIPLY] = OMG_SCANCODE_KP_MULTIPLY;
    this->scancode_map[KEY_KP_SUBTRACT] = OMG_SCANCODE_KP_MINUS;
    this->scancode_map[KEY_KP_ADD] = OMG_SCANCODE_KP_PLUS;
    this->scancode_map[KEY_KP_ENTER] = OMG_SCANCODE_KP_ENTER;
    this->scancode_map[KEY_KP_EQUAL] = OMG_SCANCODE_KP_EQUALS;
    this->scancode_map[KEY_BACK] = OMG_SCANCODE_AC_BACK;
    this->scancode_map[KEY_MENU] = OMG_SCANCODE_APPLICATION; // Why?
    this->scancode_map[KEY_VOLUME_UP] = OMG_SCANCODE_VOLUMEUP;
    this->scancode_map[KEY_VOLUME_DOWN] = OMG_SCANCODE_VOLUMEDOWN;
    this->scancode_map[KEY_A] = OMG_SCANCODE_A;
    this->scancode_map[KEY_B] = OMG_SCANCODE_B;
    this->scancode_map[KEY_C] = OMG_SCANCODE_C;
    this->scancode_map[KEY_D] = OMG_SCANCODE_D;
    this->scancode_map[KEY_E] = OMG_SCANCODE_E;
    this->scancode_map[KEY_F] = OMG_SCANCODE_F;
    this->scancode_map[KEY_G] = OMG_SCANCODE_G;
    this->scancode_map[KEY_H] = OMG_SCANCODE_H;
    this->scancode_map[KEY_I] = OMG_SCANCODE_I;
    this->scancode_map[KEY_J] = OMG_SCANCODE_J;
    this->scancode_map[KEY_K] = OMG_SCANCODE_K;
    this->scancode_map[KEY_L] = OMG_SCANCODE_L;
    this->scancode_map[KEY_M] = OMG_SCANCODE_M;
    this->scancode_map[KEY_N] = OMG_SCANCODE_N;
    this->scancode_map[KEY_O] = OMG_SCANCODE_O;
    this->scancode_map[KEY_P] = OMG_SCANCODE_P;
    this->scancode_map[KEY_Q] = OMG_SCANCODE_Q;
    this->scancode_map[KEY_R] = OMG_SCANCODE_R;
    this->scancode_map[KEY_S] = OMG_SCANCODE_S;
    this->scancode_map[KEY_T] = OMG_SCANCODE_T;
    this->scancode_map[KEY_U] = OMG_SCANCODE_U;
    this->scancode_map[KEY_V] = OMG_SCANCODE_V;
    this->scancode_map[KEY_W] = OMG_SCANCODE_W;
    this->scancode_map[KEY_X] = OMG_SCANCODE_X;
    this->scancode_map[KEY_Y] = OMG_SCANCODE_Y;
    this->scancode_map[KEY_Z] = OMG_SCANCODE_Z;
    if (OMG_ISNULL(base->clock)) {
        base->clock = OMG_MALLOC(base->mem, sizeof(OMG_ClockRaylib));
        if (OMG_ISNULL(base->clock)) {
            OMG_FREE(base->mem, this->scancode_map);
            this->scancode_map = NULL;
            return true;
        }
        base->clock->was_allocated = true;
        base->clock->omg = this;
        base->clock->inited = false;
        ((OMG_ClockRaylib*)base->clock)->raylib = this->raylib;
        OMG_BEGIN_POINTER_CAST();
        base->clock->init = omg_clock_raylib_init;
        OMG_END_POINTER_CAST();
    }
    _OMG_LOG_INFO(base, "Omega successfully inited with Raylib backend");
    base->inited = true;
    return false;
}

bool omg_raylib_app_quit(OMG_OmegaRaylib* this) {
    if (base->inited) {
        omg_app_quit((OMG_Omega*)this);
        this->scancode_map = NULL;
        base->inited = false;
    }
    return false;
}

bool omg_raylib_audio_alloc(OMG_OmegaRaylib* this) {
    if (base->audio_type == OMG_AUDIO_TYPE_AUTO) {
        base->audio_type = OMG_AUDIO_TYPE_RAYLIB;
    }
    OMG_BEGIN_POINTER_CAST();
    if (base->audio_type == OMG_AUDIO_TYPE_RAYLIB) {
        if (OMG_ISNULL(base->audio)) {
            base->audio = OMG_MALLOC(base->mem, sizeof(OMG_AudioRaylib));
            if (OMG_ISNULL(base->audio))
                return omg_dummy_audio_alloc(base);
            base->audio->was_allocated = true;
        }
        else
            base->audio->was_allocated = false;
        omg_audio_fill_on_create(base->audio);
        base->audio->omg = this;
        audio_raylib->raylib = this->raylib;
        base->audio->init = omg_audio_raylib_init;
        return false;
    }
    OMG_END_POINTER_CAST();
    return omg_audio_alloc(base);
}

bool omg_raylib_destroy(OMG_OmegaRaylib* this) {
    bool result = base->app_quit((OMG_Omega*)this);
    if (base->should_free_std) {
        result = OMG_FREE(base->mem, base->std) || result;
        base->std = NULL;
    }
    if (base->should_free_mem) {
        result = base->mem->destroy(base->mem) || result;
        base->mem = NULL;
    }
    if (this->should_free_raylib) {
        result = omg_raylib_dll_free(this->raylib) || result;
        this->raylib = NULL;
    }
    omg_destroy((OMG_Omega*)this);
    return result;
}

void omg_raylib_delay(OMG_OmegaRaylib* this, double seconds) {
    this->raylib->WaitTime(seconds);
}

bool omg_raylib_init(OMG_OmegaRaylib* this) {
    if (OMG_ISNULL(this->raylib)) {
        this->raylib = &this->raylib_stk;
        if (omg_raylib_dll_load(this->raylib, NULL))
            return true;
        this->should_free_raylib = true;
    }
    else
        this->should_free_raylib = false;
    OMG_BEGIN_POINTER_CAST();
    omg_omg_init(this);
    base->type = OMG_OMEGA_TYPE_RAYLIB;
    if (OMG_ISNULL(base->mem)) {
        base->mem = omg_memory_raylib_create(this, this->raylib);
        if (OMG_ISNULL(base->mem)) {
            if (this->should_free_raylib) {
                omg_raylib_dll_free(this->raylib);
                this->raylib = NULL;
            }
            return true;
        }
        base->should_free_mem = true;
    }
    else
        base->should_free_mem = false;
    if (OMG_ISNULL(base->std)) {
        base->std = OMG_MALLOC(base->mem, sizeof(OMG_Std));
        if (OMG_ISNULL(base->std)) {
            if (this->should_free_raylib) {
                omg_raylib_dll_free(this->raylib);
                this->raylib = NULL;
            }
            return true;
        }
        omg_std_fill_defaults(base->std);
        omg_std_set_default_handle(base->std);
        omg_raylib_fill_std(this);
        base->should_free_std = true;
    }
    else
        base->should_free_std = false;
    base->log_str_type = omg_raylib_log_str_type;
    base->auto_loop_run = omg_raylib_auto_loop_run;
    base->app_init = omg_raylib_app_init;
    base->app_quit = omg_raylib_app_quit;
    base->delay = omg_raylib_delay;
    base->audio_alloc = omg_raylib_audio_alloc;
    base->winmgr_alloc = omg_raylib_alloc_winmgr;
    base->destroy = omg_raylib_destroy;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
