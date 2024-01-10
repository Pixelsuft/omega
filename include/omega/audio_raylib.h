#pragma once
#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/audio.h>

typedef struct {
    OMG_Music parent;
    Music mus;
} OMG_MusicRaylib;

typedef struct {
    OMG_Sound parent;
    Sound sound;
} OMG_SoundRaylib;

typedef struct {
    OMG_Audio parent;
    OMG_Raylib* raylib;
    OMG_MusicRaylib* mus_play_cache[OMG_MAX_PLAYING_MUSIC];
} OMG_AudioRaylib;

OMG_API bool omg_audio_raylib_init(OMG_AudioRaylib* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_raylib_update(OMG_AudioRaylib* this);
OMG_API bool omg_audio_raylib_destroy(OMG_AudioRaylib* this);
OMG_API bool omg_audio_raylib_mus_set_volume(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, float volume);
OMG_API bool omg_audio_raylib_mus_play(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, int loops, double pos, double fade_in);
OMG_API bool omg_audio_raylib_mus_destroy(OMG_AudioRaylib* this, OMG_MusicRaylib* mus);
OMG_API bool omg_audio_raylib_mus_stop(OMG_AudioRaylib* this, OMG_MusicRaylib* mus);
OMG_API OMG_MusicRaylib* omg_audio_raylib_mus_from_fp(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, const OMG_String* path);
#endif
#endif
