#pragma once

#define OMG_AUDIO_TYPE_NONE 0
#define OMG_AUDIO_TYPE_SDL2 0
#define OMG_AUDIO_TYPE_RAYLIB 0
#define OMG_AUDIO_TYPE_FMOD 0

typedef struct {
    int type;
} OMG_Audio;
