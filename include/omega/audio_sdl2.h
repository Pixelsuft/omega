#pragma once
#include <omega/api_sdl2_mixer.h>

#if OMG_SUPPORT_SDL2_MIXER
#include <omega/audio.h>

typedef struct {
    OMG_Music base;
    Mix_Music* mus;
} OMG_MusicSdl2;

typedef struct {
    OMG_Audio parent;
    OMG_Sdl2* sdl2;
    OMG_Sdl2Mixer mix;
} OMG_AudioSdl2;

OMG_API bool omg_audio_sdl2_init(OMG_AudioSdl2* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_sdl2_mus_destroy(OMG_AudioSdl2* this, OMG_MusicSdl2* mus);
OMG_API OMG_MusicSdl2* omg_audio_sdl2_mus_from_fp(OMG_AudioSdl2* this, OMG_MusicSdl2* mus, const OMG_String* path);
OMG_API bool omg_audio_sdl2_destroy(OMG_AudioSdl2* this);
#endif
#endif
