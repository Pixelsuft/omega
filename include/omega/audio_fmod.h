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
    void* temp_buf;
    FMOD_SOUND* mus;
    FMOD_CHANNEL* channel;
    float pitch_cache;
    float vol_cache;
} OMG_MusicFmod;

OMG_API bool omg_audio_fmod_init(OMG_AudioFmod* this);
OMG_API bool omg_audio_fmod_mus_play(OMG_AudioFmod* this, OMG_MusicFmod* mus, int loops, double pos, double fade_in);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_fmod_mus_set_volume(OMG_AudioFmod* this, OMG_MusicFmod* mus, float volume);
OMG_API bool omg_audio_fmod_mus_destroy(OMG_AudioFmod* this, OMG_MusicFmod* mus);
OMG_API bool omg_audio_fmod_mus_stop(OMG_AudioFmod* this, OMG_MusicFmod* mus);
OMG_API OMG_MusicFmod* omg_audio_fmod_mus_from_fp(OMG_AudioFmod* this, OMG_MusicFmod* mus, const OMG_String* path, int format);
OMG_API OMG_MusicFmod* omg_audio_fmod_mus_from_mem(OMG_AudioFmod* this, OMG_MusicFmod* mus, const void* data, size_t size, int format);
OMG_API OMG_MusicFmod* omg_audio_fmod_mus_from_file(OMG_AudioFmod* this, OMG_MusicFmod* mus, OMG_File* file, bool destroy_file, int format);
OMG_API bool omg_audio_fmod_update(OMG_AudioFmod* this);
OMG_API bool omg_audio_fmod_destroy(OMG_AudioFmod* this);
OMG_API OMG_MusicFmod* omg_audio_fmod_snd_from_fp(OMG_AudioFmod* this, OMG_MusicFmod* mus, const OMG_String* path, int format);
OMG_API OMG_MusicFmod* omg_audio_fmod_snd_from_mem(OMG_AudioFmod* this, OMG_MusicFmod* mus, const void* data, size_t size, int format);
OMG_API FMOD_RESULT omg_audio_fmod_mus_callback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2);
OMG_API double omg_audio_fmod_mus_get_pos(OMG_AudioFmod* this, OMG_MusicFmod* mus);
OMG_API bool omg_audio_fmod_mus_set_pos(OMG_AudioFmod* this, OMG_MusicFmod* mus, double pos);
OMG_API bool omg_audio_fmod_mus_set_speed(OMG_AudioFmod* this, OMG_MusicFmod* mus, float speed);
#endif
#endif
