#pragma once
#include <omega/api_fmod.h>

#if OMG_SUPPORT_FMOD
#include <omega/audio.h>

typedef struct {
    OMG_Audio parent;
    OMG_Fmod fmod;
    FMOD_SYSTEM* sys;
} OMG_AudioFmod;

OMG_API bool omg_audio_fmod_init(OMG_AudioFmod* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_fmod_update(OMG_AudioFmod* this);
OMG_API bool omg_audio_fmod_destroy(OMG_AudioFmod* this);
#endif
#endif
