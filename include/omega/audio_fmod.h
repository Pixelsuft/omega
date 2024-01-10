#pragma once
#include <omega/api_fmod.h>

#if OMG_SUPPORT_FMOD
#include <omega/audio.h>

typedef struct {
    OMG_Audio parent;
    OMG_Fmod fmod;
    FMOD_SYSTEM* sys;
} OMG_AudioFmod;

typedef struct {
    OMG_Music parent;
    FMOD_SOUND* mus;
    FMOD_CHANNEL* channel;
    float vol_cache;
} OMG_MusicFmod;

OMG_API bool omg_audio_fmod_init(OMG_AudioFmod* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_fmod_mus_set_volume(OMG_AudioFmod* this, OMG_MusicFmod* mus, float volume);
OMG_API bool omg_audio_fmod_mus_play(OMG_AudioFmod* this, OMG_MusicFmod* mus, int loops, double pos, double fade_in);
OMG_API bool omg_audio_fmod_mus_destroy(OMG_AudioFmod* this, OMG_MusicFmod* mus);
OMG_API bool omg_audio_fmod_mus_stop(OMG_AudioFmod* this, OMG_MusicFmod* mus);
OMG_MusicFmod* omg_audio_fmod_mus_from_fp(OMG_AudioFmod* this, OMG_MusicFmod* mus, const OMG_String* path);
OMG_API bool omg_audio_fmod_update(OMG_AudioFmod* this);
OMG_API bool omg_audio_fmod_destroy(OMG_AudioFmod* this);
#endif
#endif
