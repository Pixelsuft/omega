#pragma once
#include <omega/ostd.h>

#if OMG_NULL_MUS_CHECK
#define _OMG_NULL_MUS_CHECK() do { \
    if (OMG_ISNULL(mus)) \
        return true; \
} while (0)
#else
#define _OMG_NULL_MUS_CHECK()
#endif

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

typedef struct OMG_Music {
    double duration;
    bool was_allocated;
} OMG_Music;

typedef struct OMG_Audio {
    void* omg;
    void* extra1;
    bool (*init)(struct OMG_Audio* this);
    bool (*destroy)(struct OMG_Audio* this);
    OMG_Music* (*mus_from_fp)(struct OMG_Audio* this, OMG_Music* mus, const OMG_String* path);
    bool (*mus_destroy)(struct OMG_Audio* this, OMG_Music* mus);
    bool (*mus_play)(struct OMG_Audio* this, OMG_Music* mus, int loops, double pos, double fade_in);
    const OMG_String* dev_name;
    int init_flags;
    int freq;
    int type;
    int channels;
    int chunk_size;
    bool allow_freq_change;
    bool allow_format_change;
    bool allow_channels_change;
    bool allow_samples_change;
    bool use_float32;
    bool inited;
    bool was_allocated;
} OMG_Audio;

OMG_API void omg_audio_fill_on_create(OMG_Audio* this);
OMG_API bool omg_audio_init(OMG_Audio* this);
OMG_API bool omg_audio_destroy(OMG_Audio* this);
OMG_API bool omg_audio_mus_destroy(OMG_Audio* this, OMG_Music* mus);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_mus_play(OMG_Audio* this, OMG_Music* mus, int loops, double pos, double fade_in);
OMG_API OMG_Music* omg_audio_mus_from_fp(OMG_Audio* this, OMG_Music* mus, const OMG_String* path);
#endif
