#include <omega/omega_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/memory_raylib.h>
#include <omega/winmgr_raylib.h>
#include <omega/window_raylib.h>
#include <omega/clock_raylib.h>
#define base ((OMG_Omega*)this)
#define winmgr_raylib ((OMG_WinmgrRaylib*)base->winmgr)
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
    static OMG_OmegaRaylib result;
    omg_raylib_fill_after_create(&result, data);
    return &result;
}

void omg_raylib_fill_std(OMG_OmegaRaylib* this) {
    base->std->memory_allocator = base->mem;
}

bool omg_raylib_log_info_str(OMG_OmegaRaylib* this, const OMG_String* data) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    this->raylib->TraceLog(LOG_INFO, data->ptr);
    return false;
}

bool omg_raylib_log_warn_str(OMG_OmegaRaylib* this, const OMG_String* data) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    this->raylib->TraceLog(LOG_WARNING, data->ptr);
    return false;
}

bool omg_raylib_log_error_str(OMG_OmegaRaylib* this, const OMG_String* data) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    this->raylib->TraceLog(LOG_ERROR, data->ptr);
    return false;
}

bool omg_raylib_log_fatal_str(OMG_OmegaRaylib* this, const OMG_String* data) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    this->raylib->TraceLog(LOG_FATAL, data->ptr);
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
            OMG_EventQuit event;
            MAKE_EVENT(&event);
            base->on_quit(&event);
        }
    }
    if (this->raylib->IsWindowResized()) {
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
    if (OMG_ISNULL(base->clock)) {
        base->clock = OMG_MALLOC(base->mem, sizeof(OMG_ClockRaylib));
        if (OMG_ISNULL(base->clock)) {
            return true;
        }
        base->clock->was_allocated = true;
        base->clock->omg = this;
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
        base->inited = false;
    }
    return false;
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
    base->log_info_str = omg_raylib_log_info_str;
    base->log_warn_str = omg_raylib_log_warn_str;
    base->log_error_str = omg_raylib_log_error_str;
    base->log_fatal_str = omg_raylib_log_fatal_str;
    base->auto_loop_run = omg_raylib_auto_loop_run;
    base->app_init = omg_raylib_app_init;
    base->app_quit = omg_raylib_app_quit;
    base->delay = omg_raylib_delay;
    base->winmgr_alloc = omg_raylib_alloc_winmgr;
    base->destroy = omg_raylib_destroy;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
