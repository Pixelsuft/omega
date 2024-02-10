#pragma once
#include <omega/config.h>

#if OMG_SUPPORT_EMSCRIPTEN_AUDIO
#include <omega/audio.h>

typedef struct {
    OMG_Audio parent;
} OMG_AudioEm;

typedef struct {
    OMG_Music parent;
    int id;
} OMG_MusicEm;

OMG_API bool omg_audio_emscripten_init(OMG_AudioEm* this);
#if OMG_EXPORT_SHIT
OMG_API bool omg_audio_emscripten_mus_set_volume(OMG_AudioEm* this, OMG_MusicEm* mus, float volume);
OMG_API bool omg_audio_emscripten_mus_stop(OMG_AudioEm* this, OMG_MusicEm* mus);
OMG_API bool omg_audio_emscripten_mus_play(OMG_AudioEm* this, OMG_MusicEm* mus, int loops, double pos, double fade_in);
#endif
#endif
