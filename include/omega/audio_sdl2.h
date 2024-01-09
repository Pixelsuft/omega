#pragma once
#include <omega/api_sdl2_mixer.h>

#if OMG_SUPPORT_SDL2_MIXER
#include <omega/audio.h>

typedef struct {
    OMG_Audio parent;
    OMG_Sdl2* sdl2;
    OMG_Sdl2Mixer mix;
} OMG_AudioSdl2;

OMG_API bool omg_audio_sdl2_init(OMG_AudioSdl2* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_sdl2_destroy(OMG_AudioSdl2* this);
#endif
#endif
