#pragma once

#define OMG_AUDIO_TYPE_NONE 0
#define OMG_AUDIO_TYPE_SDL2 1
#define OMG_AUDIO_TYPE_RAYLIB 2
#define OMG_AUDIO_TYPE_FMOD 3

#define OMG_AUDIO_FORMAT_FLAC (1 << 1)
#define OMG_AUDIO_FORMAT_MOD (1 << 2)
#define OMG_AUDIO_FORMAT_MP3 (1 << 3)
#define OMG_AUDIO_FORMAT_OGG (1 << 4)
#define OMG_AUDIO_FORMAT_MID (1 << 5)
#define OMG_AUDIO_FORMAT_OPUS (1 << 6)

typedef struct {
    int init_flags;
    int type;
} OMG_Audio;
