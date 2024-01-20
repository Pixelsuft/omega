#include <omega/audio.h>
#include <omega/omega.h>
#define omg_base ((OMG_Omega*)this->omg)

bool omg_audio_destroy(OMG_Audio* this) {
    OMG_UNUSED(this);
    return false;
}

void omg_audio_fill_on_create(OMG_Audio* this) {
    this->inited = false;
    this->init_flags = OMG_AUDIO_FORMAT_MP3 | OMG_AUDIO_FORMAT_OGG | OMG_AUDIO_FORMAT_WAV;
    this->type = OMG_AUDIO_TYPE_NONE;
    this->init = omg_audio_init;
    this->freq = -1;
    this->channels = -1;
    this->chunk_size = -1;
    this->dev_name = NULL;
    this->allow_freq_change = true;
    this->allow_format_change = true;
    this->allow_channels_change = true;
    this->allow_samples_change = true;
    this->use_float32 = true;
}

bool omg_audio_update(OMG_Audio* this) {
    OMG_UNUSED(this);
    return false;
}

OMG_Music* omg_audio_dummy_mus_alloc(OMG_Audio* this, OMG_Music* mus) {
#ifdef OMG_ALLOW_DUMMY_MUSIC
    OMG_UNUSED(this);
    if (OMG_ISNULL(mus)) {
        static OMG_Music mus_buf;
        mus = &mus_buf;
    }
    mus->was_allocated = false;
    return mus;
#else
    OMG_UNUSED(this, mus);
    return NULL;
#endif
}

OMG_Sound* omg_audio_dummy_snd_alloc(OMG_Audio* this, OMG_Sound* snd) {
#ifdef OMG_ALLOW_DUMMY_MUSIC
    OMG_UNUSED(this);
    if (OMG_ISNULL(snd)) {
        static OMG_Sound snd_buf;
        snd = &snd_buf;
    }
    snd->was_allocated = false;
    return snd;
#else
    OMG_UNUSED(this, snd);
    return NULL;
#endif
}

OMG_Music* omg_audio_mus_from_fp(OMG_Audio* this, OMG_Music* mus, const OMG_String* path) {
    OMG_UNUSED(path);
    return omg_audio_dummy_mus_alloc(this, mus);
}

bool omg_audio_mus_destroy(OMG_Audio* this, OMG_Music* mus) {
    if (OMG_ISNULL(mus))
        return false;
    if (mus->was_allocated) {
        mus->was_allocated = false;
        OMG_FREE(omg_base->mem, mus);
    }
    return false;
}

bool omg_audio_mus_play(OMG_Audio* this, OMG_Music* mus, int loops, double pos, double fade_in) {
    OMG_UNUSED(this, mus, loops, pos, fade_in);
    return false;
}

bool omg_audio_mus_set_volume(OMG_Audio* this, OMG_Music* mus, float volume) {
    OMG_UNUSED(this, mus, volume);
    return false;
}

bool omg_audio_mus_stop(OMG_Audio* this, OMG_Music* mus) {
    OMG_UNUSED(this, mus);
    return false;
}

bool omg_audio_snd_destroy(OMG_Audio* this, OMG_Sound* snd) {
    if (OMG_ISNULL(snd))
        return false;
    if (snd->was_allocated) {
        snd->was_allocated = false;
        OMG_FREE(omg_base->mem, snd);
    }
    return false;
}

OMG_Sound* omg_audio_snd_from_fp(OMG_Audio* this, OMG_Sound* snd, const OMG_String* path) {
    OMG_UNUSED(path);
    return omg_audio_dummy_snd_alloc(this, snd);
}

bool omg_audio_snd_set_volume(OMG_Audio* this, OMG_Sound* snd, float volume) {
    OMG_UNUSED(this, snd, volume);
    return false;
}

bool omg_audio_snd_play(OMG_Audio* this, OMG_Sound* snd, int loops, double fade_in) {
    OMG_UNUSED(this, snd, loops, fade_in);
    return false;
}

bool omg_audio_snd_stop(OMG_Audio* this, OMG_Sound* snd) {
    OMG_UNUSED(this, snd);
    return false;
}

double omg_audio_mus_get_pos(OMG_Audio* this, OMG_Music* mus) {
    OMG_UNUSED(this, mus);
    return -1.0;
}

bool omg_audio_mus_set_pos(OMG_Audio* this, OMG_Music* mus, double pos) {
    OMG_UNUSED(this, mus, pos);
    return false;
}

bool omg_audio_mus_set_speed(OMG_Audio* this, OMG_Music* mus, double pos) {
    OMG_UNUSED(this, mus, pos);
    return false;
}

bool omg_audio_init(OMG_Audio* this) {
    this->inited = false;
    this->type = OMG_AUDIO_TYPE_NONE;
    this->destroy = omg_audio_destroy;
    this->update = omg_audio_update;
    this->mus_from_fp = omg_audio_mus_from_fp;
    this->mus_destroy = omg_audio_mus_destroy;
    this->mus_play = omg_audio_mus_play;
    this->mus_set_volume = omg_audio_mus_set_volume;
    this->mus_stop = omg_audio_mus_stop;
    this->mus_get_pos = omg_audio_mus_get_pos;
    this->mus_set_pos = omg_audio_mus_set_pos;
    this->mus_set_speed = omg_audio_mus_set_speed;
    this->snd_destroy = omg_audio_snd_destroy;
    this->snd_from_fp = omg_audio_snd_from_fp;
    this->snd_set_volume = omg_audio_snd_set_volume;
    this->snd_play = omg_audio_snd_play;
    this->snd_stop = omg_audio_snd_stop;
    return false;
}
