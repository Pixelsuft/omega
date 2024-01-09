#pragma once
#include <omega/api_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/audio.h>

typedef struct {
    OMG_Audio parent;
    OMG_Raylib* raylib;
} OMG_AudioRaylib;

OMG_API bool omg_audio_raylib_init(OMG_AudioRaylib* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_raylib_destroy(OMG_AudioRaylib* this);
#endif
#endif
