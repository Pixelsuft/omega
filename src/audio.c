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

OMG_Music* omg_audio_mus_from_fp(OMG_Audio* this, OMG_Music* mus, const OMG_String* path, int format) {
    OMG_UNUSED(path, format);
    return omg_audio_dummy_mus_alloc(this, mus);
}

OMG_Music* omg_audio_mus_from_mem(OMG_Audio* this, OMG_Music* mus, const void* data, size_t size, int format) {
    OMG_UNUSED(data, size, format);
    return omg_audio_dummy_mus_alloc(this, mus);
}

OMG_Music* omg_audio_mus_from_file(OMG_Audio* this, OMG_Music* mus, OMG_File* file, bool destroy_file, int format) {
    if (OMG_ISNULL(file)) {
        _OMG_LOG_WARN(omg_base, "Null pointer passed for creating music from file");
        return omg_audio_dummy_mus_alloc(this, mus);
    }
    int64_t file_size = file->get_size(file);
    if (file_size <= 0) {
        if (destroy_file)
            file->destroy(file);
        return omg_audio_dummy_mus_alloc(this, mus);
    }
    void* buf = OMG_MALLOC(omg_base->mem, file_size);
    if (OMG_ISNULL(buf)) {
        if (destroy_file)
            file->destroy(file);
        return omg_audio_dummy_mus_alloc(this, mus);
    }
    size_t size_read = file->read(file, buf, 1, (size_t)file_size);
    if (destroy_file)
        file->destroy(file);
    if (size_read == 0) {
        OMG_FREE(omg_base->mem, buf);
        return omg_audio_dummy_mus_alloc(this, mus);
    }
    OMG_Music* res = this->mus_from_mem(this, mus, buf, size_read, format | OMG_AUDIO_FORMAT_INTERNAL);
    // OMG_FREE(omg_base->mem, buf);
    return res;
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
    return true;
}

bool omg_audio_mus_set_volume(OMG_Audio* this, OMG_Music* mus, float volume) {
    OMG_UNUSED(this, mus, volume);
    return true;
}

bool omg_audio_mus_stop(OMG_Audio* this, OMG_Music* mus) {
    OMG_UNUSED(this, mus);
    return true;
}

bool omg_audio_mus_pause(OMG_Audio* this, OMG_Music* mus, bool paused) {
    OMG_UNUSED(this, mus, paused);
    return false;
}

bool omg_audio_snd_pause(OMG_Audio* this, OMG_Sound* snd, bool paused) {
    OMG_UNUSED(this, snd, paused);
    return true;
}

bool omg_audio_snd_set_speed(OMG_Audio* this, OMG_Sound* snd, float speed) {
    OMG_UNUSED(this, snd, speed);
    return true;
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

OMG_Sound* omg_audio_snd_from_fp(OMG_Audio* this, OMG_Sound* snd, const OMG_String* path, int format) {
    OMG_UNUSED(path, format);
    return omg_audio_dummy_snd_alloc(this, snd);
}

OMG_Sound* omg_audio_snd_from_mem(OMG_Audio* this, OMG_Sound* snd, const void* data, size_t size, int format) {
    OMG_UNUSED(data, size, format);
    return omg_audio_dummy_snd_alloc(this, snd);
}

OMG_Sound* omg_audio_snd_from_file(OMG_Audio* this, OMG_Sound* snd, OMG_File* file, bool destroy_file, int format) {
    if (OMG_ISNULL(file)) {
        _OMG_LOG_WARN(omg_base, "Null pointer passed for creating sound from file");
        return omg_audio_dummy_snd_alloc(this, snd);
    }
    int64_t file_size = file->get_size(file);
    if (file_size <= 0) {
        if (destroy_file)
            file->destroy(file);
        return omg_audio_dummy_snd_alloc(this, snd);
    }
    void* buf = OMG_MALLOC(omg_base->mem, file_size);
    if (OMG_ISNULL(buf)) {
        if (destroy_file)
            file->destroy(file);
        return omg_audio_dummy_snd_alloc(this, snd);
    }
    size_t size_read = file->read(file, buf, 1, (size_t)file_size);
    if (destroy_file)
        file->destroy(file);
    if (size_read == 0) {
        OMG_FREE(omg_base->mem, buf);
        return omg_audio_dummy_snd_alloc(this, snd);
    }
    OMG_Sound* res = this->snd_from_mem(this, snd, buf, size_read, format);
    OMG_FREE(omg_base->mem, buf);
    return res;
}

bool omg_audio_snd_set_volume(OMG_Audio* this, OMG_Sound* snd, float volume) {
    OMG_UNUSED(this, snd, volume);
    return true;
}

bool omg_audio_snd_play(OMG_Audio* this, OMG_Sound* snd, int loops, double fade_in) {
    OMG_UNUSED(this, snd, loops, fade_in);
    return true;
}

bool omg_audio_snd_stop(OMG_Audio* this, OMG_Sound* snd) {
    OMG_UNUSED(this, snd);
    return true;
}

double omg_audio_mus_get_pos(OMG_Audio* this, OMG_Music* mus) {
    OMG_UNUSED(this, mus);
    return -1.0;
}

bool omg_audio_mus_set_pos(OMG_Audio* this, OMG_Music* mus, double pos) {
    OMG_UNUSED(this, mus, pos);
    return true;
}

bool omg_audio_mus_set_speed(OMG_Audio* this, OMG_Music* mus, float speed) {
    OMG_UNUSED(this, mus, speed);
    return true;
}

bool omg_audio_init(OMG_Audio* this) {
    this->inited = false;
    this->type = OMG_AUDIO_TYPE_NONE;
    this->destroy = omg_audio_destroy;
    this->update = omg_audio_update;
    this->mus_from_fp = omg_audio_mus_from_fp;
    this->mus_from_mem = omg_audio_mus_from_mem;
    this->mus_destroy = omg_audio_mus_destroy;
    this->mus_play = omg_audio_mus_play;
    this->mus_set_volume = omg_audio_mus_set_volume;
    this->mus_stop = omg_audio_mus_stop;
    this->mus_pause = omg_audio_mus_pause;
    this->mus_get_pos = omg_audio_mus_get_pos;
    this->mus_set_pos = omg_audio_mus_set_pos;
    this->mus_set_speed = omg_audio_mus_set_speed;
    this->snd_destroy = omg_audio_snd_destroy;
    this->snd_from_fp = omg_audio_snd_from_fp;
    this->snd_from_mem = omg_audio_snd_from_mem;
    this->snd_set_volume = omg_audio_snd_set_volume;
    this->snd_play = omg_audio_snd_play;
    this->snd_stop = omg_audio_snd_stop;
    this->snd_pause = omg_audio_snd_pause;
    this->snd_set_speed = omg_audio_snd_set_speed;
    OMG_BEGIN_POINTER_CAST();
    this->mus_from_file = omg_audio_mus_from_file;
    this->snd_from_file = omg_audio_snd_from_file;
    OMG_END_POINTER_CAST();
    return false;
}
