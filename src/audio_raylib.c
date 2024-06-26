#include <omega/audio_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define snd_base ((OMG_Sound*)snd)
#define omg_base ((OMG_Omega*)base->omg)

#define _AUDIO_TYPE_RAYLIB(audio_type, format) do { \
    if ((format & OMG_AUDIO_FORMAT_AUTO) || (format & OMG_AUDIO_FORMAT_NONE) || (format & OMG_AUDIO_FORMAT_OGG)) \
        audio_type = ".ogg"; \
    else if (format & OMG_AUDIO_FORMAT_FLAC) \
        audio_type = ".flac"; \
    else if (format & OMG_AUDIO_FORMAT_MOD) \
        audio_type = ".mod"; \
    else if (format & OMG_AUDIO_FORMAT_MP3) \
        audio_type = ".mp3"; \
    else if (format & OMG_AUDIO_FORMAT_MID) \
        audio_type = ".midi"; \
    else if (format & OMG_AUDIO_FORMAT_OPUS) \
        audio_type = ".opus"; \
    else \
        audio_type = ".wav"; \
} while (0)

bool omg_audio_raylib_destroy(OMG_AudioRaylib* this) {
    if (!base->inited)
        return false;
    this->raylib->CloseAudioDevice();
    return false;
}

bool omg_audio_raylib_update(OMG_AudioRaylib* this) {
    for (size_t i = 0; i < OMG_MAX_PLAYING_MUSIC; i++) {
        if (OMG_ISNOTNULL(this->mus_play_cache[i]) && this->raylib->IsMusicStreamPlaying(this->mus_play_cache[i]->mus)) {
            this->raylib->UpdateMusicStream(this->mus_play_cache[i]->mus);
        }
    }
    return false;
}

bool omg_audio_raylib_mus_set_volume(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, float volume) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return true;
    this->raylib->SetMusicVolume(mus->mus, volume);
    return false;
}

bool omg_audio_raylib_mus_play(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, int loops, double pos, double fade_in) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return true;
    OMG_UNUSED(fade_in);
    mus->mus.looping = loops == -1;
    if (this->raylib->IsMusicStreamPlaying(mus->mus)) {
        this->raylib->SeekMusicStream(mus->mus, (float)pos);
        return false;
    }
    if (pos > 0.0)
        this->raylib->SeekMusicStream(mus->mus, (float)pos);
    for (size_t i = 0; i < OMG_MAX_PLAYING_MUSIC; i++) {
        if (OMG_ISNULL(this->mus_play_cache[i]) || !this->raylib->IsMusicStreamPlaying(this->mus_play_cache[i]->mus)) {
            this->mus_play_cache[i] = mus;
            break;
        }
    }
    this->raylib->PlayMusicStream(mus->mus);
    return false;
}

bool omg_audio_raylib_mus_destroy(OMG_AudioRaylib* this, OMG_MusicRaylib* mus) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return false;
    for (size_t i = 0; i < OMG_MAX_PLAYING_MUSIC; i++) {
        if (this->mus_play_cache[i] == mus) {
            this->mus_play_cache[i] = NULL;
            break;
        }
    }
    this->raylib->UnloadMusicStream(mus->mus);
    if (OMG_ISNOTNULL(mus->temp_buf)) {
        OMG_FREE(omg_base->mem, mus->temp_buf);
        mus->temp_buf = NULL;
    }
    omg_audio_mus_destroy(base, mus_base);
    return false;
}

bool omg_audio_raylib_mus_stop(OMG_AudioRaylib* this, OMG_MusicRaylib* mus) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return true;
    this->raylib->StopMusicStream(mus->mus);
    return false;
}

double omg_audio_raylib_mus_get_pos(OMG_AudioRaylib* this, OMG_MusicRaylib* mus) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return -1.0;
    return (double)this->raylib->GetMusicTimePlayed(mus->mus);
}

bool omg_audio_raylib_mus_set_speed(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, float speed) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return true;
    this->raylib->SetMusicPitch(mus->mus, speed);
    return false;
}

OMG_MusicRaylib* omg_audio_raylib_mus_from_fp(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, const OMG_String* path, int format) {
    OMG_UNUSED(format);
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicRaylib));
        if (OMG_ISNULL(mus))
            return (OMG_MusicRaylib*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    mus->mus = this->raylib->LoadMusicStream(path->ptr);
    if (!this->raylib->IsMusicReady(mus->mus)) {
        omg_audio_mus_destroy(base, mus_base);
        _OMG_LOG_ERROR(omg_base, "Failed to load Raylib music ", path->ptr);
        return (OMG_MusicRaylib*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->temp_buf = NULL;
    mus_base->is_dummy = false;
    mus_base->duration = (double)this->raylib->GetMusicTimeLength(mus->mus);
    return mus;
}

OMG_MusicRaylib* omg_audio_raylib_mus_from_mem(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, const void* data, size_t size, int format) {
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicRaylib));
        if (OMG_ISNULL(mus))
            return (OMG_MusicRaylib*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    if (format & OMG_AUDIO_FORMAT_INTERNAL)
        mus->temp_buf = (void*)data;
    else
        mus->temp_buf = NULL;
    char* mus_type;
    _AUDIO_TYPE_RAYLIB(mus_type, format);
    mus->mus = this->raylib->LoadMusicStreamFromMemory(mus_type, (const unsigned char*)data, (int)size);
    if (!this->raylib->IsMusicReady(mus->mus)) {
        omg_audio_mus_destroy(base, mus_base);
        _OMG_LOG_ERROR(omg_base, "Failed to load Raylib music from memory");
        return (OMG_MusicRaylib*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus_base->is_dummy = false;
    mus_base->duration = (double)this->raylib->GetMusicTimeLength(mus->mus);
    return mus;
}

bool omg_audio_raylib_snd_destroy(OMG_AudioRaylib* this, OMG_SoundRaylib* snd) {
    if (OMG_IS_DUMMY_AUDIO(snd_base))
        return false;
    this->raylib->UnloadSound(snd->snd);
    omg_audio_snd_destroy(base, snd_base);
    return false;
}

OMG_SoundRaylib* omg_audio_raylib_snd_from_fp(OMG_AudioRaylib* this, OMG_SoundRaylib* snd, const OMG_String* path, int format) {
    OMG_UNUSED(format);
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(snd)) {
        snd = OMG_MALLOC(omg_base->mem, sizeof(OMG_SoundRaylib));
        if (OMG_ISNULL(snd))
            return (OMG_SoundRaylib*)omg_audio_dummy_snd_alloc(base, snd_base);
        snd_base->was_allocated = true;
    }
    else
        snd_base->was_allocated = false;
    snd->snd = this->raylib->LoadSound(path->ptr);
    if (!this->raylib->IsSoundReady(snd->snd)) {
        omg_audio_snd_destroy(base, snd_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open sound ", path->ptr);
        return (OMG_SoundRaylib*)omg_audio_dummy_snd_alloc(base, snd_base);
    }
    snd_base->is_dummy = false;
    snd_base->duration = -1.0;
    return snd;
}

OMG_SoundRaylib* omg_audio_raylib_snd_from_mem(OMG_AudioRaylib* this, OMG_SoundRaylib* snd, const void* data, size_t size, int format) {
    if (OMG_ISNULL(snd)) {
        snd = OMG_MALLOC(omg_base->mem, sizeof(OMG_SoundRaylib));
        if (OMG_ISNULL(snd))
            return (OMG_SoundRaylib*)omg_audio_dummy_snd_alloc(base, snd_base);
        snd_base->was_allocated = true;
    }
    else
        snd_base->was_allocated = false;
    char* audio_type;
    _AUDIO_TYPE_RAYLIB(audio_type, format);
    Wave snd_wave = this->raylib->LoadWaveFromMemory(audio_type, (const unsigned char*)data, (int)size);
    if (!this->raylib->IsWaveReady(snd_wave)) {
        omg_audio_snd_destroy(base, snd_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open sound wave from mem");
        return (OMG_SoundRaylib*)omg_audio_dummy_snd_alloc(base, snd_base);
    }
    snd->snd = this->raylib->LoadSoundFromWave(snd_wave);
    this->raylib->UnloadWave(snd_wave);
    if (!this->raylib->IsSoundReady(snd->snd)) {
        omg_audio_snd_destroy(base, snd_base);
        _OMG_LOG_ERROR(omg_base, "Failed to open sound from mem");
        return (OMG_SoundRaylib*)omg_audio_dummy_snd_alloc(base, snd_base);
    }
    snd_base->is_dummy = false;
    snd_base->duration = -1.0;
    return snd;
}

bool omg_audio_raylib_snd_set_volume(OMG_AudioRaylib* this, OMG_SoundRaylib* snd, float volume) {
    if (OMG_IS_DUMMY_AUDIO(snd_base))
        return true;
    this->raylib->SetSoundVolume(snd->snd, volume);
    return false;
}

bool omg_audio_raylib_mus_set_pos(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, double pos) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return true;
    if (pos < 0.0)
        pos = 0.0;
    this->raylib->SeekMusicStream(mus->mus, (float)pos);
    return false;
}

bool omg_audio_raylib_snd_play(OMG_AudioRaylib* this, OMG_SoundRaylib* snd, int loops, double pos, double fade_in) {
    if (OMG_IS_DUMMY_AUDIO(snd_base))
        return true;
    OMG_UNUSED(loops, pos, fade_in);
    this->raylib->PlaySound(snd->snd);
    return false;
}

bool omg_audio_raylib_snd_stop(OMG_AudioRaylib* this, OMG_SoundRaylib* snd) {
    if (OMG_IS_DUMMY_AUDIO(snd_base))
        return true;
    this->raylib->StopSound(snd->snd);
    return false;
}

bool omg_audio_raylib_mus_pause(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, bool paused) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return true;
    (paused ? this->raylib->PauseMusicStream : this->raylib->ResumeMusicStream)(mus->mus);
    return false;
}

bool omg_audio_raylib_snd_pause(OMG_AudioRaylib* this, OMG_SoundRaylib* snd, bool paused) {
    if (OMG_IS_DUMMY_AUDIO(snd_base))
        return true;
    (paused ? this->raylib->PauseSound : this->raylib->ResumeSound)(snd->snd);
    return false;
}

bool omg_audio_raylib_snd_set_speed(OMG_AudioRaylib* this, OMG_SoundRaylib* snd, float speed) {
    if (OMG_IS_DUMMY_AUDIO(snd_base))
        return true;
    this->raylib->SetSoundPitch(snd->snd, speed);
    return false;
}

bool omg_audio_raylib_mus_set_panning(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, float left, float right) {
    if (OMG_IS_DUMMY_AUDIO(mus_base))
        return true;
    left /= (left + right);
    this->raylib->SetMusicPan(mus->mus, left);
    return false;
}

bool omg_audio_raylib_snd_set_panning(OMG_AudioRaylib* this, OMG_SoundRaylib* snd, float left, float right) {
    if (OMG_IS_DUMMY_AUDIO(snd_base))
        return true;
    left /= (left + right);
    this->raylib->SetSoundPan(snd->snd, left);
    return false;
}

bool omg_audio_raylib_init(OMG_AudioRaylib* this) {
    omg_audio_init(base);
    this->raylib->InitAudioDevice();
    if (!this->raylib->IsAudioDeviceReady()) {
        _OMG_LOG_ERROR(omg_base, "Failed to init Raylib audio device");
        return true;
    }
    omg_base->std->memset(this->mus_play_cache, 0, sizeof(OMG_MusicRaylib*) * OMG_MAX_PLAYING_MUSIC);
    OMG_BEGIN_POINTER_CAST();
    base->update = omg_audio_raylib_update;
    base->destroy = omg_audio_raylib_destroy;
    base->mus_from_fp = omg_audio_raylib_mus_from_fp;
    base->mus_from_mem = omg_audio_raylib_mus_from_mem;
    base->mus_destroy = omg_audio_raylib_mus_destroy;
    base->mus_play = omg_audio_raylib_mus_play;
    base->mus_stop = omg_audio_raylib_mus_stop;
    base->mus_pause = omg_audio_raylib_mus_pause;
    base->mus_set_volume = omg_audio_raylib_mus_set_volume;
    base->mus_get_pos = omg_audio_raylib_mus_get_pos;
    base->mus_set_pos = omg_audio_raylib_mus_set_pos;
    base->mus_set_speed = omg_audio_raylib_mus_set_speed;
    base->mus_set_panning = omg_audio_raylib_mus_set_panning;
    base->snd_from_fp = omg_audio_raylib_snd_from_fp;
    base->snd_from_mem = omg_audio_raylib_snd_from_mem;
    base->snd_destroy = omg_audio_raylib_snd_destroy;
    base->snd_set_volume = omg_audio_raylib_snd_set_volume;
    base->snd_play = omg_audio_raylib_snd_play;
    base->snd_stop = omg_audio_raylib_snd_stop;
    base->snd_pause = omg_audio_raylib_snd_pause;
    base->snd_set_speed = omg_audio_raylib_snd_set_speed;
    base->snd_set_panning = omg_audio_raylib_snd_set_panning;
    OMG_END_POINTER_CAST();
    base->type = OMG_AUDIO_TYPE_RAYLIB;
    base->inited = true;
    return false;
}
#endif
