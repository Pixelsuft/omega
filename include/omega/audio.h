#pragma once
#include <omega/ostd.h>

#define OMG_AUDIO_TYPE_NONE 0
#define OMG_AUDIO_TYPE_AUTO 1
#define OMG_AUDIO_TYPE_SDL2 2
#define OMG_AUDIO_TYPE_RAYLIB 3
#define OMG_AUDIO_TYPE_FMOD 4

#define OMG_AUDIO_FORMAT_FLAC (1 << 1)
#define OMG_AUDIO_FORMAT_MOD (1 << 2)
#define OMG_AUDIO_FORMAT_MP3 (1 << 3)
#define OMG_AUDIO_FORMAT_OGG (1 << 4)
#define OMG_AUDIO_FORMAT_MID (1 << 5)
#define OMG_AUDIO_FORMAT_OPUS (1 << 6)
#define OMG_AUDIO_FORMAT_WAV (1 << 7)

typedef struct OMG_Audio {
    void* omg;
    void* extra1;
    bool (*init)(struct OMG_Audio* this);
    bool (*destroy)(struct OMG_Audio* this);
    int init_flags;
    int type;
    bool inited;
    bool was_allocated;
} OMG_Audio;

OMG_API void omg_audio_fill_on_create(OMG_Audio* this);
OMG_API bool omg_audio_init(OMG_Audio* this);
OMG_API bool omg_audio_destroy(OMG_Audio* this);
