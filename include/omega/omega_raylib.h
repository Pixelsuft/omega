#pragma once
#include <omega/omega.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/api_raylib.h>

typedef struct {
    OMG_Omega parent;
    OMG_Raylib raylib_stk;
    OMG_Raylib* raylib;
    uint32_t* scancode_map;
    bool should_free_raylib;
    bool is_focused;
} OMG_OmegaRaylib;

OMG_API OMG_OmegaRaylib* omg_raylib_create(OMG_EntryData* data);
OMG_API bool omg_raylib_init(OMG_OmegaRaylib* this);
OMG_API void omg_raylib_fill_std(OMG_OmegaRaylib* this);
OMG_API void omg_raylib_fill_after_create(OMG_OmegaRaylib* this, OMG_EntryData* data);
OMG_API bool omg_raylib_destroy(OMG_OmegaRaylib* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_raylib_alloc_winmgr(OMG_OmegaRaylib* this);
OMG_API void omg_raylib_auto_loop_run(OMG_OmegaRaylib* this);
OMG_API void omg_raylib_poll_events(OMG_OmegaRaylib* this);
OMG_API bool omg_raylib_app_init(OMG_OmegaRaylib* this);
OMG_API bool omg_raylib_app_quit(OMG_OmegaRaylib* this);
OMG_API void omg_raylib_delay(OMG_OmegaRaylib* this, double seconds);
OMG_API bool omg_raylib_audio_alloc(OMG_OmegaRaylib* this);
OMG_API bool omg_raylib_log_str_type(OMG_OmegaRaylib* this, const OMG_String* data, int type);
#endif
#endif
