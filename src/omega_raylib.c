#include <omega/omega_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/memory_raylib.h>
#include <omega/window_raylib.h>
#define base ((OMG_Omega*)this)
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

OMG_WindowRaylib* omg_raylib_window_alloc(OMG_OmegaRaylib* this) {
    OMG_WindowRaylib* result = OMG_MALLOC(base->mem, sizeof(OMG_WindowRaylib));
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(base, "Failed to allocate memory for Raylib Window");
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_window_fill_on_create(result);
    result->parent.omg = base;
    result->parent.default_init = omg_window_raylib_init;
    result->raylib = this->raylib;
    result->parent.was_allocated = true;
    OMG_END_POINTER_CAST();
    return result;
}

void omg_raylib_poll_events(OMG_OmegaRaylib* this) {
    if (this->raylib->WindowShouldClose()) {
        OMG_EventQuit event;
        MAKE_EVENT(&event);
        base->on_quit(&event);
    }
}

void omg_raylib_auto_loop_run(OMG_OmegaRaylib* this) {
    base->looping = true;
    while (base->looping) {
        omg_raylib_poll_events(this);
        if (!base->looping)
            break;
        OMG_EventUpdate u_event;
        MAKE_EVENT(&u_event);
        base->on_update(&u_event);
    }
    OMG_EventLoopStop ls_event;
    MAKE_EVENT(&ls_event);
    base->on_loop_stop(&ls_event);
}

bool omg_raylib_destroy(OMG_OmegaRaylib* this) {
    bool result = false;
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
    base->window_alloc = omg_raylib_window_alloc;
    base->destroy = omg_raylib_destroy;
    OMG_END_POINTER_CAST();
    _OMG_LOG_INFO(base, "Omega successfully inited with Raylib backend");
    return false;
}
#endif
