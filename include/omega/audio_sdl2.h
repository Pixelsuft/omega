#pragma once
#include <omega/api_sdl2_mixer.h>

#if OMG_SUPPORT_SDL2_MIXER
#include <omega/audio.h>

typedef struct {
    OMG_Audio parent;
    OMG_Sdl2* sdl2;
    OMG_Sdl2Mixer* mix;
} OMG_AudioSdl2;
#endif
