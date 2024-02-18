#pragma once
#include <omega/omega.h>

#if OMG_SUPPORT_SDL2
#include <omega/api_sdl2.h>

typedef struct {
    OMG_Omega parent;
    OMG_Sdl2 sdl2_stk;
    OMG_EventUpdate u_event;
    OMG_EventPaint p_event;
    SDL_Event ev;
    OMG_Sdl2* sdl2;
    bool should_free_sdl2;
    bool not_prevent_close;
} OMG_OmegaSdl2;

OMG_API OMG_OmegaSdl2* omg_sdl2_create(OMG_EntryData* data);
OMG_API bool omg_sdl2_init(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_fill_std(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_fill_after_create(OMG_OmegaSdl2* this, OMG_EntryData* data);
OMG_API bool omg_sdl2_destroy(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_poll_events(OMG_OmegaSdl2* this);
#if OMG_EXPORT_SHIT
#include <omega/window_sdl2.h>
#include <omega/filesystem_sdl2.h>

OMG_API bool omg_sdl2_alloc_winmgr(OMG_OmegaSdl2* this);
OMG_API bool omg_sdl2_app_init(OMG_OmegaSdl2* this);
OMG_API bool omg_sdl2_app_quit(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_delay(OMG_OmegaSdl2* this, double seconds);
OMG_API int omg_sdl2_set_text_input_state(OMG_OmegaSdl2* this, int state);
OMG_API void omg_sdl2_auto_loop_run(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_poll_events(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_auto_loop_on_stop(OMG_OmegaSdl2* this);
OMG_API void omg_sdl2_auto_loop_tick(OMG_OmegaSdl2* this);
OMG_API bool omg_sdl2_file_destroy(OMG_FileSdl2* file);
OMG_API OMG_FileSdl2* omg_sdl2_file_from_fp(OMG_OmegaSdl2* this, OMG_FileSdl2* file, const OMG_String* path, int mode);
OMG_API int64_t omg_sdl2_file_seek(OMG_FileSdl2* file, int64_t offset, int whence);
OMG_API int64_t omg_sdl2_file_get_size(OMG_FileSdl2* file) ;
OMG_API size_t omg_sdl2_file_read(OMG_FileSdl2* file, void* buf, size_t size, size_t maxnum);
OMG_API size_t omg_sdl2_file_write(OMG_FileSdl2* file, const void* buf, size_t size, size_t num);
OMG_API bool omg_sdl2_log_str_type(OMG_OmegaSdl2* this, const OMG_String* data, int type);
OMG_API bool omg_sdl2_audio_alloc(OMG_OmegaSdl2* this);
OMG_API OMG_String omg_sdl2_env_get(OMG_OmegaSdl2* this, const OMG_String* key_name);
OMG_API bool omg_sdl2_env_set(OMG_OmegaSdl2* this, const OMG_String* key_name, const OMG_String* key_value, bool overwrite);
#endif
#endif
