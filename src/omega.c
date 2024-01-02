#include <omega/omega.h>

static OMG_Omega* omg_def_omega = NULL;

OMG_API OMG_Omega* omg_get_default_omega(void) {
    return omg_def_omega;
}

OMG_Omega* omg_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    return NULL;
}

void omg_delay(OMG_Omega* this, double seconds) {
    OMG_UNUSED(this, seconds);
}

void omg_fill_on_create(OMG_Omega* this, OMG_EntryData* data) {
    this->entry_data = data;
    this->mem = NULL;
    this->winmgr = NULL;
    this->log_level = this->log_level_omg = this->log_level_lib = -1;
    this->sdl2_dll_path = NULL;
    this->log_info_str = NULL;
    this->log_warn_str = NULL;
    this->log_error_str = NULL;
    this->log_fatal_str = NULL;
    this->support_highdpi = true;
    this->app_theme = OMG_THEME_AUTO;
    this->omg_init = omg_omg_init;
}

void omg_log_set_level(OMG_Omega* this, const int log_level, const int omg_log_level, const int lib_log_level) {
    if (log_level >= 0)
        this->log_level = log_level;
    if (omg_log_level >= 0)
        this->log_level_omg = omg_log_level;
    if (lib_log_level >= 0)
        this->log_level_lib = lib_log_level;
}

bool omg_log_info_str(OMG_Omega* this, const OMG_String* data) {
    OMG_UNUSED(this, data);
    return false;
}

bool omg_destroy(OMG_Omega* this) {
    OMG_UNUSED(this);
    omg_def_omega = NULL;
    return false;
}

bool omg_app_init(OMG_Omega* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_alloc_winmgr(OMG_Omega* this) {
    if (OMG_ISNULL(this->winmgr))
        return false;
    this->winmgr->omg = this;
    this->winmgr->was_allocated = true;
    this->winmgr->init = omg_winmgr_init;
    return false;
}

bool omg_free_winmgr(OMG_Omega* this) {
    if (OMG_ISNULL(this->winmgr))
        return false;
    this->winmgr->destroy(this->winmgr);
    if (this->winmgr->was_allocated) {
        OMG_FREE(this->mem, this->winmgr);
        this->winmgr = NULL;
    }
    return false;
}

bool omg_app_quit(OMG_Omega* this) {
    omg_free_winmgr(this);
    if (OMG_ISNOTNULL(this->clock) && this->clock->was_allocated) {
        this->clock->destroy(this->clock);
        OMG_FREE(this->mem, this->clock);
        this->clock = NULL;
    }
    if (OMG_ISNOTNULL(this->keyboard_state)) {
        OMG_FREE(this->mem, this->keyboard_state);
        this->keyboard_state = NULL;
        this->keymap = NULL;
    }
    return false;
}

void omg_auto_loop_run(OMG_Omega* this) {
    this->looping = false;
}

void omg_auto_loop_stop(OMG_Omega* this) {
    this->looping = false;
}

int omg_set_text_input_state(OMG_Omega* this, int state) {
    OMG_UNUSED(this, state);
    return OMG_TEXT_INPUT_DISABLED;
}

void omg_event_on_quit(OMG_EventQuit* event) {
    OMG_Omega* this = OMG_OMEGA_FROM_EVENT(event);
    this->auto_loop_stop(this);
}

void omg_event_on_update(OMG_EventUpdate* event) {
    OMG_UNUSED(event);
}

void omg_event_on_paint(OMG_EventPaint* event) {
    OMG_UNUSED(event);
}

void omg_event_on_expose(OMG_EventExpose* event) {
    OMG_UNUSED(event);
}

void omg_event_on_resize(OMG_EventResize* event) {
    OMG_UNUSED(event);
}

void omg_event_on_loop_stop(OMG_EventLoopStop* event) {
    OMG_UNUSED(event);
}

void omg_event_on_state_change(OMG_EventStateChange* event) {
    OMG_UNUSED(event);
}

void omg_event_on_focus_change(OMG_EventFocusChange* event) {
    OMG_UNUSED(event);
}

void omg_event_on_mouse_wheel(OMG_EventMouseWheel* event) {
    OMG_UNUSED(event);
}

void omg_event_on_state_changing(OMG_EventStateChanging* event) {
    OMG_Window* this = OMG_WIN_FROM_EVENT(event);
    if (event->change == OMG_WIN_STATE_CLOSED) {
        if (!(this->sys_buttons & OMG_WIN_SYS_BUTTON_CLOSE))
            event->allow = false;
    }
    else if (event->change == OMG_WIN_STATE_MAXIMIZED) {
        if (!(this->sys_buttons & OMG_WIN_SYS_BUTTON_MAXIMIZE))
            event->allow = false;
    }
    else if (event->change == OMG_WIN_STATE_MINIMIZED) {
        if (!(this->sys_buttons & OMG_WIN_SYS_BUTTON_MINIMIZE))
            event->allow = false;
    }
}

void omg_event_on_close(OMG_EventClose* event) {
    OMG_UNUSED(event);
}

void omg_event_on_mouse_move(OMG_EventMouseMove* event) {
    OMG_UNUSED(event);
}

void omg_event_on_mouse_button(OMG_EventMouseButton* event) {
    OMG_UNUSED(event);
}

void omg_event_on_mouse_focus(OMG_EventMouseFocus* event) {
    OMG_UNUSED(event);
}

void omg_event_on_keyboard(OMG_EventKeyboard* event) {
    OMG_UNUSED(event);
}

void omg_event_on_text_input(OMG_EventTextInput* event) {
    OMG_UNUSED(event);
}

void omg_reset_event_handlers(OMG_Omega* this) {
    this->on_quit = omg_event_on_quit;
    this->on_update = omg_event_on_update;
    this->on_loop_stop = omg_event_on_loop_stop;
    this->on_paint = omg_event_on_paint;
    this->on_expose = omg_event_on_expose;
    this->on_close = omg_event_on_close;
    this->on_resize = omg_event_on_resize;
    this->on_size_change = omg_event_on_resize;
    this->on_state_change = omg_event_on_state_change;
    this->on_state_changing = omg_event_on_state_changing;
    this->on_mouse_move = omg_event_on_mouse_move;
    this->on_mouse_down = omg_event_on_mouse_button;
    this->on_mouse_up = omg_event_on_mouse_button;
    this->on_mouse_wheel = omg_event_on_mouse_wheel;
    this->on_mouse_enter = omg_event_on_mouse_focus;
    this->on_mouse_leave = omg_event_on_mouse_focus;
    this->on_focus_change = omg_event_on_focus_change;
    this->on_key_down = omg_event_on_keyboard;
    this->on_key_up = omg_event_on_keyboard;
    this->on_text_input = omg_event_on_text_input;
}

bool omg_omg_init(OMG_Omega* this) {
    this->type = OMG_OMEGA_TYPE_NONE;
    this->extra1 = this->extra2 = this->extra3 = this->extra4 = this->extra5 = NULL;
    this->std = NULL;
    this->clock = NULL;
    this->keyboard_state = NULL;
    this->keymap = NULL;
    this->looping = false;
    this->emulate_mouse = true;
    this->enable_paint = true;
    this->supports_screen_keyboard = false;
    this->should_free_mem = false;
    this->theme = OMG_THEME_NONE;
    this->scale.x = this->scale.y = 1.0f;
    if (this->log_level == -1)
        this->log_level = OMG_LOG_MIN_LEVEL;
    if (this->log_level_omg == -1)
        this->log_level_omg = OMG_LOG_OMG_MIN_LEVEL;
    // TODO: probably also handle long functions
    if (OMG_ISNULL(this->log_info_str))
        this->log_info_str = omg_log_info_str;
    if (OMG_ISNULL(this->log_warn_str))
        this->log_warn_str = omg_log_info_str;
    if (OMG_ISNULL(this->log_error_str))
        this->log_error_str = omg_log_info_str;
    if (OMG_ISNULL(this->log_fatal_str))
        this->log_fatal_str = omg_log_info_str;
    this->log_set_level = omg_log_set_level;
    this->destroy = omg_destroy;
    this->app_init = omg_app_init;
    this->app_quit = omg_app_quit;
    this->auto_loop_run = omg_auto_loop_run;
    this->auto_loop_stop = omg_auto_loop_stop;
    this->set_text_input_state = omg_set_text_input_state;
    this->delay = omg_delay;
    this->reset_event_handlers = omg_reset_event_handlers;
    this->winmgr_alloc = omg_alloc_winmgr;
    omg_reset_event_handlers(this);
    omg_def_omega = this;
    return false;
}
