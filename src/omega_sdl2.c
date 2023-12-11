#include <omega/omega_sdl2.h>

#if OMG_SUPPORT_SDL2
#include <omega/memory_sdl2.h>
#include <omega/window_sdl2.h>
#define base ((OMG_Omega*)this)

void omg_sdl2_fill_after_create(OMG_OmegaSdl2* this) {
    this->sdl2 = NULL;
    OMG_BEGIN_POINTER_CAST();
    omg_fill_on_create(this);
    base->default_init = omg_sdl2_init;
    OMG_END_POINTER_CAST();
}

OMG_OmegaSdl2* omg_sdl2_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    static OMG_OmegaSdl2 result;
    omg_sdl2_fill_after_create(&result);
    return &result;
}

void omg_sdl2_fill_std(OMG_OmegaSdl2* this) {
    base->std->memory_allocator = base->mem;
    base->std->memset = this->sdl2->SDL_memset;
    base->std->memcpy = this->sdl2->SDL_memcpy;
    base->std->memmove = this->sdl2->SDL_memmove;
    base->std->memcmp = this->sdl2->SDL_memcmp;
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

OMG_Window* omg_sdl2_window_create(OMG_Omega* this) {
    OMG_Window* result = OMG_MALLOC(this->mem, sizeof(OMG_WindowSdl2));
    if (OMG_ISNULL(result)) {
        _OMG_LOG_ERROR(this, "Failed to allocate memory for SDL2 Window");
        return NULL;
    }
    OMG_BEGIN_POINTER_CAST();
    result->omg = this;
    result->default_init = omg_window_sdl2_init;
    OMG_END_POINTER_CAST();
    return result;
}

bool omg_sdl2_destroy(OMG_OmegaSdl2* this) {
    bool result = false;
    if (this->inited) {
        this->sdl2->SDL_Quit();
    }
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
    this->inited = false;
    if (OMG_ISNULL(this->sdl2)) {
        this->sdl2 = &this->sdl2_stk;
        if (omg_sdl2_dll_load(this->sdl2, NULL))
            return true;
        this->should_free_sdl2 = true;
    }
    else
        this->should_free_sdl2 = false;
    if (this->sdl2->SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
        if (this->should_free_sdl2) {
            omg_sdl2_dll_free(this->sdl2);
            this->sdl2 = NULL;
        }
        return true;
    }
    OMG_BEGIN_POINTER_CAST();
    omg_init(this);
    base->type = OMG_OMEGA_TYPE_SDL2;
    if (OMG_ISNULL(base->mem)) {
        base->mem = omg_memory_sdl2_create(this, this->sdl2);
        if (OMG_ISNULL(base->mem)) {
            this->sdl2->SDL_Quit();
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
            this->sdl2->SDL_Quit();
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
    base->log_info_str = omg_sdl2_log_info_str;
    base->log_warn_str = omg_sdl2_log_warn_str;
    base->log_error_str = omg_sdl2_log_error_str;
    base->log_fatal_str = omg_sdl2_log_fatal_str;
    base->window_create = omg_sdl2_window_create;
    base->destroy = omg_sdl2_destroy;
    OMG_END_POINTER_CAST();
    this->inited = true;
    _OMG_LOG_INFO(base, "Omega successfully inited with SDL2 backend");
    return false;
}
#endif
