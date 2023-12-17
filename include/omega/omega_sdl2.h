#pragma once
#include <omega/omega.h>

#if OMG_SUPPORT_SDL2
#include <omega/api_sdl2.h>

typedef struct {
    OMG_Omega parent;
    OMG_Sdl2 sdl2_stk;
    OMG_Sdl2* sdl2;
    bool should_free_sdl2;
} OMG_OmegaSdl2;

OMG_API OMG_OmegaSdl2* omg_sdl2_create(OMG_EntryData* data);
OMG_API bool omg_sdl2_init(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_fill_std(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_fill_after_create(OMG_OmegaSdl2* this, OMG_EntryData* data);
OMG_API bool omg_sdl2_destroy(OMG_OmegaSdl2* this);
#if OMG_EXPORT_SHIT
#include <omega/window_sdl2.h>

OMG_API OMG_WindowSdl2* omg_sdl2_window_alloc(OMG_OmegaSdl2* this);
OMG_API bool omg_sdl2_app_init(OMG_OmegaSdl2* this);
OMG_API bool omg_sdl2_app_quit(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_delay(OMG_OmegaSdl2* this, float seconds);
OMG_API bool omg_sdl2_log_info_str(OMG_OmegaSdl2* this, const OMG_String* data);
OMG_API bool omg_sdl2_log_warn_str(OMG_OmegaSdl2* this, const OMG_String* data);
OMG_API bool omg_sdl2_log_error_str(OMG_OmegaSdl2* this, const OMG_String* data);
OMG_API bool omg_sdl2_log_fatal_str(OMG_OmegaSdl2* this, const OMG_String* data);
#endif
#endif
