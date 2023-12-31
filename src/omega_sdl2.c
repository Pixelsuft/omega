#include <omega/omega_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/memory_sdl2.h>
#include <omega/window_sdl2.h>
#include <omega/winmgr_sdl2.h>
#include <omega/clock_sdl2.h>
#define base ((OMG_Omega*)this)
#define winmgr_sdl2 ((OMG_WinmgrSdl2*)base->winmgr)
#define MAKE_EVENT(event) do { \
    ((OMG_Event*)event)->omg = this; \
    ((OMG_Event*)event)->data = base->event_arg; \
    ((OMG_Event*)event)->time = (uint64_t)(this->ev.common.timestamp); \
} while (0)
#define MAKE_EVENT_STATIC(event) do { \
    ((OMG_Event*)event)->omg = this; \
    ((OMG_Event*)event)->data = base->event_arg; \
    ((OMG_Event*)event)->time = this->sdl2->SDL_GetTicks64(); \
} while (0)
#define FIND_SDL2_WIN(win, window_id) for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) { \
    if (OMG_ISNOTNULL(base->winmgr->cache[i]) && (((OMG_WindowSdl2*)base->winmgr->cache[i])->id == window_id)) { \
        win = base->winmgr->cache[i]; \
        break; \
    } \
} \

void omg_sdl2_fill_after_create(OMG_OmegaSdl2* this, OMG_EntryData* data) {
    this->sdl2 = NULL;
    OMG_BEGIN_POINTER_CAST();
    omg_fill_on_create(this, data);
    base->omg_init = omg_sdl2_init;
    OMG_END_POINTER_CAST();
}

OMG_OmegaSdl2* omg_sdl2_create(OMG_EntryData* data) {
    static OMG_OmegaSdl2 result;
    omg_sdl2_fill_after_create(&result, data);
    return &result;
}

void omg_sdl2_fill_std(OMG_OmegaSdl2* this) {
    base->std->memory_allocator = base->mem;
    base->std->memset = this->sdl2->SDL_memset;
    base->std->memcpy = this->sdl2->SDL_memcpy;
    base->std->memmove = this->sdl2->SDL_memmove;
    base->std->memcmp = this->sdl2->SDL_memcmp;
    base->std->strcmp = this->sdl2->SDL_strcmp;
    base->std->strlen = this->sdl2->SDL_strlen;
    // base->std->strnlen = this->sdl2->SDL_strnlen;
    base->std->wcslen = this->sdl2->SDL_wcslen;
    base->std->strrev = this->sdl2->SDL_strrev;
    base->std->itoa = this->sdl2->SDL_itoa;
    base->std->uitoa = this->sdl2->SDL_uitoa;
    base->std->ltoa = this->sdl2->SDL_ltoa;
    base->std->ultoa = this->sdl2->SDL_ultoa;
    base->std->lltoa = this->sdl2->SDL_lltoa;
    base->std->ulltoa = this->sdl2->SDL_ulltoa;
    base->std->utf8strlen = this->sdl2->SDL_utf8strlen;
    base->std->utf8strnlen = this->sdl2->SDL_utf8strnlen;
    base->std->floor = this->sdl2->SDL_floor;
    base->std->ceil = this->sdl2->SDL_ceil;
    base->std->round = this->sdl2->SDL_round;
    base->std->floorf = this->sdl2->SDL_floorf;
    base->std->ceilf = this->sdl2->SDL_ceilf;
    base->std->roundf = this->sdl2->SDL_roundf;
}

bool omg_sdl2_log_info_str(OMG_OmegaSdl2* this, const OMG_String* data) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    this->sdl2->SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, data->ptr);
    return false;
}

bool omg_sdl2_log_warn_str(OMG_OmegaSdl2* this, const OMG_String* data) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    this->sdl2->SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, data->ptr);
    return false;
}

bool omg_sdl2_log_error_str(OMG_OmegaSdl2* this, const OMG_String* data) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    this->sdl2->SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, data->ptr);
    return false;
}

bool omg_sdl2_log_fatal_str(OMG_OmegaSdl2* this, const OMG_String* data) {
    if (omg_string_ensure_null((OMG_String*)data))
        return true;
    this->sdl2->SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, data->ptr);
    return false;
}

void omg_sdl2_delay(OMG_OmegaSdl2* this, double seconds) {
    this->sdl2->SDL_Delay((uint32_t)(seconds * 1000.0));
}

void omg_sdl2_poll_events(OMG_OmegaSdl2* this) {
    while (this->sdl2->SDL_PollEvent(&this->ev)) {
        switch (this->ev.type) {
            case SDL_MOUSEMOTION: {
                OMG_Window* win = NULL;
                FIND_SDL2_WIN(win, this->ev.motion.windowID);
                if (OMG_ISNULL(win))
                    break;
                OMG_EventMouseMove event;
                MAKE_EVENT(&event);
                event.is_emulated = this->ev.motion.which == SDL_TOUCH_MOUSEID;
                if (!event.is_emulated || base->emulate_mouse) {
                    event.win = win;
                    event.id = this->ev.motion.which;
                    event.state = this->ev.motion.state;
                    event.pos.x = (float)this->ev.motion.x;
                    event.pos.y = (float)this->ev.motion.y;
                    event.rel.x = (float)this->ev.motion.xrel;
                    event.rel.y = (float)this->ev.motion.yrel;
                    base->on_mouse_move(&event);
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                OMG_Window* win = NULL;
                FIND_SDL2_WIN(win, this->ev.button.windowID);
                if (OMG_ISNULL(win))
                    break;
                OMG_EventMouseButton event;
                MAKE_EVENT(&event);
                event.is_emulated = this->ev.button.which == SDL_TOUCH_MOUSEID;
                if (!event.is_emulated || base->emulate_mouse) {
                    event.win = win;
                    event.id = this->ev.button.which;
                    event.button = this->ev.button.button;
                    event.pos.x = (float)this->ev.button.x;
                    event.pos.y = (float)this->ev.button.y;
                    event.clicks = this->ev.button.clicks;
                    event.is_pressed = this->ev.button.state == SDL_PRESSED;
                    (this->ev.type == SDL_MOUSEBUTTONDOWN ? base->on_mouse_down : base->on_mouse_up)(&event);
                }
                break;
            }
            case SDL_WINDOWEVENT: {
                OMG_Window* win = NULL;
                FIND_SDL2_WIN(win, this->ev.window.windowID);
                if (OMG_ISNULL(win))
                    break;
                switch (this->ev.window.event) {
                    case SDL_WINDOWEVENT_ENTER: {
                        // TODO
                        int x, y;
                        uint32_t state = this->sdl2->SDL_GetMouseState(&x, &y);
                        OMG_UNUSED(x, y, state);
                        break;
                    }
                    case SDL_WINDOWEVENT_LEAVE: {
                        int x, y;
                        uint32_t state = this->sdl2->SDL_GetMouseState(&x, &y);
                        OMG_UNUSED(x, y, state);
                        break;
                    }
                    case SDL_WINDOWEVENT_SIZE_CHANGED: {
                        OMG_EventResize event;
                        MAKE_EVENT(&event);
                        event.win = win;
                        win->size.w = event.size.w = (float)this->ev.window.data1;
                        win->size.h = event.size.h = (float)this->ev.window.data2;
                        if (OMG_ISNOTNULL(win->ren))
                            win->ren->_on_update_window_size(win->ren);
                        base->on_size_change(&event);
                        break;
                    }
                    case SDL_WINDOWEVENT_RESIZED: {
                        OMG_EventResize event;
                        MAKE_EVENT(&event);
                        event.win = win;
                        win->size.w = event.size.w = (float)this->ev.window.data1;
                        win->size.h = event.size.h = (float)this->ev.window.data2;
                        if (OMG_ISNOTNULL(win->ren))
                            win->ren->_on_update_window_size(win->ren);
                        base->on_resize(&event);
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_GAINED: {
                        // TODO
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_LOST: {
                        break;
                    }
                    case SDL_WINDOWEVENT_MINIMIZED: {
                        OMG_EventStateChanging c_event;
                        MAKE_EVENT(&c_event);
                        c_event.win = win;
                        c_event.allow = true;
                        c_event.change = OMG_WIN_STATE_MINIMIZED;
                        base->on_state_changing(&c_event);
                        if (!c_event.allow) {
                            this->sdl2->SDL_RestoreWindow(((OMG_WindowSdl2*)win)->win);
                            break;
                        }
                        OMG_EventStateChange event;
                        MAKE_EVENT(&event);
                        event.win = win;
                        event.prev_state = win->state;
                        event.change = OMG_WIN_STATE_MINIMIZED;
                        win->state &= ~OMG_WIN_STATE_RESTORED;
                        win->state |= OMG_WIN_STATE_MINIMIZED;
                        base->on_state_change(&event);
                        break;
                    }
                    case SDL_WINDOWEVENT_MAXIMIZED: {
                        OMG_EventStateChanging c_event;
                        MAKE_EVENT(&c_event);
                        c_event.win = win;
                        c_event.allow = true;
                        c_event.change = OMG_WIN_STATE_MAXIMIZED;
                        base->on_state_changing(&c_event);
                        if (!c_event.allow) {
                            this->sdl2->SDL_RestoreWindow(((OMG_WindowSdl2*)win)->win);
                            break;
                        }
                        OMG_EventStateChange event;
                        MAKE_EVENT(&event);
                        event.win = win;
                        event.prev_state = win->state;
                        event.change = OMG_WIN_STATE_MAXIMIZED;
                        win->state &= ~OMG_WIN_STATE_RESTORED;
                        win->state |= OMG_WIN_STATE_MAXIMIZED;
                        base->on_state_change(&event);
                        break;
                    }
                    case SDL_WINDOWEVENT_RESTORED: {
                        OMG_EventStateChanging c_event;
                        MAKE_EVENT(&c_event);
                        c_event.win = win;
                        c_event.allow = true;
                        c_event.change = OMG_WIN_STATE_RESTORED;
                        base->on_state_changing(&c_event);
                        if (!c_event.allow) {
                            (
                                (win->state & OMG_WIN_STATE_MINIMIZED) ? this->sdl2->SDL_MinimizeWindow : this->sdl2->SDL_MaximizeWindow
                            )(((OMG_WindowSdl2*)win)->win);
                            break;
                        }
                        OMG_EventStateChange event;
                        MAKE_EVENT(&event);
                        event.win = win;
                        event.prev_state = win->state;
                        event.change = OMG_WIN_STATE_RESTORED;
                        win->state &= ~OMG_WIN_STATE_MINIMIZED;
                        if (!(win->state & OMG_WIN_STATE_MINIMIZED))
                            win->state &= ~OMG_WIN_STATE_MAXIMIZED;
                        if (!(win->state & OMG_WIN_STATE_MAXIMIZED))
                            win->state |= OMG_WIN_STATE_RESTORED;
                        base->on_state_change(&event);
                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE: {
                        OMG_EventStateChanging c_event;
                        MAKE_EVENT(&c_event);
                        c_event.win = win;
                        c_event.allow = true;
                        c_event.change = OMG_WIN_STATE_CLOSED;
                        base->on_state_changing(&c_event);
                        this->not_prevent_close = c_event.allow;
                        if (c_event.allow) {
                            OMG_EventClose event;
                            MAKE_EVENT(&event);
                            event.win = win;
                            base->on_close(&event);
                        }
                        break;
                    }
                }
                break;
            }
            case SDL_QUIT: {
                if (this->not_prevent_close) {
                    OMG_EventQuit event;
                    MAKE_EVENT(&event);
                    base->on_quit(&event);
                }
                break;
            }
        }
    }
}

void omg_sdl2_auto_loop_run(OMG_OmegaSdl2* this) {
    base->looping = true;
    OMG_EventUpdate u_event;
    OMG_EventPaint p_event;
    while (base->looping) {
        omg_sdl2_poll_events(this);
        if (!base->looping)
            break;
        MAKE_EVENT_STATIC(&u_event);
        base->on_update(&u_event);
        if (base->enable_paint) {
            for (size_t i = 0; i < OMG_MAX_WINDOWS; i++) {
                if (OMG_ISNULL(base->winmgr->cache[i]) || !base->winmgr->cache[i]->enable_paint)
                    continue;
                MAKE_EVENT_STATIC(&p_event);
                p_event.win = base->winmgr->cache[i];
                base->on_paint(&p_event);
            }
        }
    }
    OMG_EventLoopStop ls_event;
    MAKE_EVENT_STATIC(&ls_event);
    base->on_loop_stop(&ls_event);
}

bool omg_sdl2_app_init(OMG_OmegaSdl2* this) {
    if (OMG_ISNULL(base->clock)) {
        base->clock = OMG_MALLOC(base->mem, sizeof(OMG_ClockSdl2));
        if (OMG_ISNULL(base->clock)) {
            return true;
        }
        base->clock->was_allocated = true;
        base->clock->omg = this;
        ((OMG_ClockSdl2*)base->clock)->sdl2 = this->sdl2;
        OMG_BEGIN_POINTER_CAST();
        base->clock->init = omg_clock_sdl2_init;
        OMG_END_POINTER_CAST();
    }
    if (this->sdl2->SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
        if (base->clock->was_allocated) {
            OMG_FREE(base->mem, base->clock);
            base->clock = NULL;
        }
        _OMG_LOG_INFO(base, "Failed to init SDL2 (", this->sdl2->SDL_GetError(), ")");
        return true;
    }
    _OMG_LOG_INFO(base, "Omega successfully inited with SDL2 backend");
    base->inited = true;
    return false;
}

bool omg_sdl2_app_quit(OMG_OmegaSdl2* this) {
    if (base->inited) {
        omg_app_quit((OMG_Omega*)this);
        this->sdl2->SDL_Quit();
        base->inited = false;
    }
    return false;
}

bool omg_sdl2_alloc_winmgr(OMG_OmegaSdl2* this) {
    base->winmgr = OMG_MALLOC(base->mem, sizeof(OMG_WinmgrSdl2));
    if (OMG_ISNULL(base->winmgr))
        return true;
    omg_alloc_winmgr((OMG_Omega*)this);
    winmgr_sdl2->sdl2 = this->sdl2;
    OMG_BEGIN_POINTER_CAST();
    base->winmgr->init = omg_winmgr_sdl2_init;
    OMG_END_POINTER_CAST();
    return false;
}

bool omg_sdl2_destroy(OMG_OmegaSdl2* this) {
    bool result = base->app_quit((OMG_Omega*)this);
    if (base->should_free_std) {
        result = OMG_FREE(base->mem, base->std) || result;
        base->std = NULL;
    }
    if (base->should_free_mem) {
        result = base->mem->destroy(base->mem) || result;
        base->mem = NULL;
    }
    if (this->should_free_sdl2) {
        result = omg_sdl2_dll_free(this->sdl2) || result;
        this->sdl2 = NULL;
    }
    omg_destroy((OMG_Omega*)this);
    return result;
}

bool omg_sdl2_init(OMG_OmegaSdl2* this) {
    base->inited = false;
    if (OMG_ISNULL(this->sdl2)) {
        this->sdl2 = &this->sdl2_stk;
        if (omg_sdl2_dll_load(this->sdl2, NULL))
            return true;
        this->should_free_sdl2 = true;
    }
    else
        this->should_free_sdl2 = false;
    OMG_BEGIN_POINTER_CAST();
    omg_omg_init(this);
    base->type = OMG_OMEGA_TYPE_SDL2;
    if (OMG_ISNULL(base->mem)) {
        base->mem = omg_memory_sdl2_create(this, this->sdl2);
        if (OMG_ISNULL(base->mem)) {
            if (this->should_free_sdl2) {
                omg_sdl2_dll_free(this->sdl2);
                this->sdl2 = NULL;
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
            if (this->should_free_sdl2) {
                omg_sdl2_dll_free(this->sdl2);
                this->sdl2 = NULL;
            }
            return true;
        }
        omg_std_fill_defaults(base->std);
        omg_std_set_default_handle(base->std);
        omg_sdl2_fill_std(this);
        base->should_free_std = true;
    }
    else
        base->should_free_std = false;
    base->app_init = omg_sdl2_app_init;
    base->app_quit = omg_sdl2_app_quit;
    base->delay = omg_sdl2_delay;
    base->log_info_str = omg_sdl2_log_info_str;
    base->log_warn_str = omg_sdl2_log_warn_str;
    base->log_error_str = omg_sdl2_log_error_str;
    base->log_fatal_str = omg_sdl2_log_fatal_str;
    base->auto_loop_run = omg_sdl2_auto_loop_run;
    base->winmgr_alloc = omg_sdl2_alloc_winmgr;
    base->destroy = omg_sdl2_destroy;
    OMG_END_POINTER_CAST();
    base->inited = true;
    return false;
}
#endif
