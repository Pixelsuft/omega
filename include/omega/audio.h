#pragma once
#include <omega/ostd.h>
#include <omega/filesystem.h>

#define OMG_IS_DUMMY_AUDIO(audio) (OMG_ISNULL((audio)) || ((audio)->is_dummy))

#define OMG_AUDIO_TYPE_NONE 0
#define OMG_AUDIO_TYPE_AUTO 1
#define OMG_AUDIO_TYPE_SDL2 2
#define OMG_AUDIO_TYPE_RAYLIB 3
#define OMG_AUDIO_TYPE_FMOD 4
#define OMG_AUDIO_TYPE_EMSCRIPTEN 5

#define OMG_AUDIO_FORMAT_NONE (1 << 0)
#define OMG_AUDIO_FORMAT_AUTO (1 << 1)
#define OMG_AUDIO_FORMAT_WAV (1 << 2)
#define OMG_AUDIO_FORMAT_FLAC (1 << 3)
#define OMG_AUDIO_FORMAT_MOD (1 << 4)
#define OMG_AUDIO_FORMAT_MP3 (1 << 5)
#define OMG_AUDIO_FORMAT_OGG (1 << 6)
#define OMG_AUDIO_FORMAT_MID (1 << 7)
#define OMG_AUDIO_FORMAT_OPUS (1 << 8)
#define OMG_AUDIO_FORMAT_INTERNAL (1 << 30)

typedef struct {
    double duration;
    bool is_dummy;
    bool was_allocated;
} OMG_Music;

typedef struct {
    double duration;
    bool is_dummy;
    bool was_allocated;
} OMG_Sound;

typedef struct OMG_Audio {
    void* omg;
    void* extra1;
    /* Init audio system */
    bool (*init)(struct OMG_Audio* this);
    /* Destroy audio system */
    bool (*destroy)(struct OMG_Audio* this);
    /* Update audio system */
    bool (*update)(struct OMG_Audio* this);
    /* Music from file path. It's recommended to set format */
    OMG_Music* (*mus_from_fp)(struct OMG_Audio* this, OMG_Music* mus, const OMG_String* path, int format);
    /* Music from memory. It's recommended to set format */
    OMG_Music* (*mus_from_mem)(struct OMG_Audio* this, OMG_Music* mus, const void* data, size_t size, int format);
    /* Music from file object. It's recommended to set format */
    OMG_Music* (*mus_from_file)(struct OMG_Audio* this, OMG_Music* mus, const OMG_File*, bool destroy_file, int format);
    /* Close music */
    bool (*mus_destroy)(struct OMG_Audio* this, OMG_Music* mus);
    /* Play music. Set loops to -1 for infinite playing */
    bool (*mus_play)(struct OMG_Audio* this, OMG_Music* mus, int loops, double pos, double fade_in);
    bool (*mus_stop)(struct OMG_Audio* this, OMG_Music* mus);
    bool (*mus_set_volume)(struct OMG_Audio* this, OMG_Music* mus, float volume);
    double (*mus_get_pos)(struct OMG_Audio* this, OMG_Music* mus);
    bool (*mus_set_pos)(struct OMG_Audio* this, OMG_Music* mus, double pos);
    bool (*mus_set_speed)(struct OMG_Audio* this, OMG_Music* mus, float speed);
    bool (*mus_pause)(struct OMG_Audio* this, OMG_Music* mus, bool paused);
    bool (*mus_set_panning)(struct OMG_Audio* this, OMG_Music* mus, float left, float right);
    /* Sound from file path */
    OMG_Sound* (*snd_from_fp)(struct OMG_Audio* this, OMG_Sound* snd, const OMG_String* path, int format);
    /* Sound from memory */
    OMG_Sound* (*snd_from_mem)(struct OMG_Audio* this, OMG_Sound* snd, const void* data, size_t size, int format);
    /* Sound from file object */
    OMG_Sound* (*snd_from_file)(struct OMG_Audio* this, OMG_Sound* snd, const OMG_File* file, bool destroy_file, int format);
    /* Close sound */
    bool (*snd_destroy)(struct OMG_Audio* this, OMG_Sound* snd);
    /* Play sound. Set loops to -1 for infinite playing */
    bool (*snd_play)(struct OMG_Audio* this, OMG_Sound* snd, int loops, double pos, double fade_in);
    bool (*snd_stop)(struct OMG_Audio* this, OMG_Sound* snd);
    bool (*snd_pause)(struct OMG_Audio* this, OMG_Sound* snd, bool paused);
    bool (*snd_set_volume)(struct OMG_Audio* this, OMG_Sound* snd, float volume);
    bool (*snd_set_speed)(struct OMG_Audio* this, OMG_Sound* snd, float speed);
    bool (*snd_set_panning)(struct OMG_Audio* this, OMG_Sound* mus, float left, float right);
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
OMG_API bool omg_audio_update(OMG_Audio* this);
OMG_API bool omg_audio_mus_destroy(OMG_Audio* this, OMG_Music* mus);
OMG_API bool omg_audio_snd_destroy(OMG_Audio* this, OMG_Sound* snd);
OMG_API OMG_Music* omg_audio_dummy_mus_alloc(OMG_Audio* this, OMG_Music* mus);
OMG_API OMG_Sound* omg_audio_dummy_snd_alloc(OMG_Audio* this, OMG_Sound* snd);
#if OMG_EXPORT_SHIT
OMG_API OMG_Sound* omg_audio_snd_from_fp(OMG_Audio* this, OMG_Sound* snd, const OMG_String* path, int format);
OMG_API OMG_Sound* omg_audio_snd_from_mem(OMG_Audio* this, OMG_Sound* snd, const void* data, size_t size, int format);
OMG_API OMG_Sound* omg_audio_snd_from_file(OMG_Audio* this, OMG_Sound* snd, OMG_File* file, bool destroy_file, int format);
OMG_API bool omg_audio_mus_set_volume(OMG_Audio* this, OMG_Music* mus, float volume);
OMG_API bool omg_audio_mus_play(OMG_Audio* this, OMG_Music* mus, int loops, double pos, double fade_in);
OMG_API bool omg_audio_mus_stop(OMG_Audio* this, OMG_Music* mus);
OMG_API bool omg_audio_mus_pause(OMG_Audio* this, OMG_Music* mus, bool paused);
OMG_API bool omg_audio_snd_set_volume(OMG_Audio* this, OMG_Sound* snd, float volume);
OMG_API bool omg_audio_snd_play(OMG_Audio* this, OMG_Sound* snd, int loops, double pos, double fade_in);
OMG_API bool omg_audio_snd_stop(OMG_Audio* this, OMG_Sound* snd);
OMG_API bool omg_audio_snd_pause(OMG_Audio* this, OMG_Sound* snd, bool paused);
OMG_API OMG_Music* omg_audio_mus_from_fp(OMG_Audio* this, OMG_Music* mus, const OMG_String* path, int format);
OMG_API OMG_Music* omg_audio_mus_from_mem(OMG_Audio* this, OMG_Music* mus, const void* data, size_t size, int format);
OMG_API OMG_Music* omg_audio_mus_from_file(OMG_Audio* this, OMG_Music* mus, OMG_File* file, bool destroy_file, int format);
OMG_API double omg_audio_mus_get_pos(OMG_Audio* this, OMG_Music* mus);
OMG_API bool omg_audio_mus_set_pos(OMG_Audio* this, OMG_Music* mus, double pos);
OMG_API bool omg_audio_mus_set_speed(OMG_Audio* this, OMG_Music* mus, float speed);
OMG_API bool omg_audio_snd_set_speed(OMG_Audio* this, OMG_Sound* snd, float speed);
OMG_API bool omg_audio_mus_set_panning(OMG_Audio* this, OMG_Music* mus, float left, float right);
OMG_API bool omg_audio_snd_set_panning(OMG_Audio* this, OMG_Sound* snd, float left, float right);
#endif
