#pragma once
#include <omega/api_sdl2_mixer.h>

#if OMG_SUPPORT_SDL2_MIXER
#include <omega/audio.h>

typedef struct {
    OMG_Music parent;
    Mix_Music* mus;
    uint64_t time_cache1;
    uint64_t time_cache2;
    int vol_cache;
    int loops_cache;
} OMG_MusicSdl2;

typedef struct {
    OMG_Music parent;
    Mix_Chunk* chunk;
    uint8_t pan_cache[2];
    int channel;
    int vol_cache;
} OMG_SoundSdl2;

typedef struct {
    OMG_Audio parent;
    OMG_SoundSdl2* play_cache[OMG_MAX_PLAYING_SOUND];
    OMG_Sdl2* sdl2;
    OMG_Sdl2Mixer mix;
    bool supports_get_pos;
} OMG_AudioSdl2;

OMG_API bool omg_audio_sdl2_init(OMG_AudioSdl2* this);
#if OMG_EXPORT_SHIT
OMG_API void omg_audio_sdl2_channel_finish_cb(int channel);
OMG_API bool omg_audio_sdl2_mus_set_volume(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, float volume);
OMG_API bool omg_audio_sdl2_mus_play(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, int loops, double pos, double fade_in);
OMG_API bool omg_audio_sdl2_mus_destroy(OMG_AudioSdl2* this, OMG_MusicSdl2* mus);
OMG_API bool omg_audio_sdl2_mus_stop(OMG_AudioSdl2* this, OMG_MusicSdl2* mus);
OMG_API double omg_audio_sdl2_mus_get_pos(OMG_AudioSdl2* this, OMG_MusicSdl2* mus);
OMG_API bool omg_audio_sdl2_mus_set_pos(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, double pos);
OMG_API OMG_MusicSdl2* omg_audio_sdl2_mus_from_fp(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, const OMG_String* path, int format);
OMG_API OMG_MusicSdl2* omg_audio_sdl2_mus_from_mem(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, const void* data, size_t size, int format);
OMG_API OMG_MusicSdl2* omg_audio_sdl2_mus_from_file(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, OMG_File* file, bool destroy_file, int format);
OMG_API bool omg_audio_sdl2_destroy(OMG_AudioSdl2* this);
OMG_API bool omg_audio_sdl2_snd_destroy(OMG_AudioSdl2* this, OMG_SoundSdl2* snd);
OMG_API OMG_SoundSdl2* omg_audio_sdl2_snd_from_fp(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, const OMG_String* path, int format);
OMG_API OMG_SoundSdl2* omg_audio_sdl2_snd_from_mem(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, const void* data, size_t size, int format);
OMG_API bool omg_audio_sdl2_snd_set_volume(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, float volume);
OMG_API bool omg_audio_sdl2_snd_play(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, int loops, double pos, double fade_in);
OMG_API bool omg_audio_sdl2_snd_stop(OMG_AudioSdl2* this, OMG_SoundSdl2* snd);
OMG_API bool omg_audio_sdl2_update(OMG_AudioSdl2* this);
OMG_API bool omg_audio_sdl2_mus_pause(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, bool paused);
OMG_API bool omg_audio_sdl2_snd_pause(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, bool paused);
OMG_API bool omg_audio_sdl2_snd_set_panning(OMG_AudioSdl2* this, OMG_SoundSdl2* snd, float left, float right);
#endif
#endif
