#include <omega/audio_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define snd_base ((OMG_Sound*)snd)
#define omg_base ((OMG_Omega*)base->omg)

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
    this->raylib->SetMusicVolume(mus->mus, volume);
    return false;
}

bool omg_audio_raylib_mus_play(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, int loops, double pos, double fade_in) {
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
    if (OMG_ISNULL(mus))
        return false;
    for (size_t i = 0; i < OMG_MAX_PLAYING_MUSIC; i++) {
        if (this->mus_play_cache[i] == mus) {
            this->mus_play_cache[i] = NULL;
            break;
        }
    }
    this->raylib->UnloadMusicStream(mus->mus);
    omg_audio_mus_destroy(base, mus_base);
    return false;
}

bool omg_audio_raylib_mus_stop(OMG_AudioRaylib* this, OMG_MusicRaylib* mus) {
    this->raylib->StopMusicStream(mus->mus);
    return false;
}

OMG_MusicRaylib* omg_audio_raylib_mus_from_fp(OMG_AudioRaylib* this, OMG_MusicRaylib* mus, const OMG_String* path) {
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicRaylib));
        if (OMG_ISNULL(mus))
            return NULL;
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    mus->mus = this->raylib->LoadMusicStream(path->ptr);
    if (!this->raylib->IsMusicReady(mus->mus)) {
        omg_audio_mus_destroy(base, mus_base);
        _OMG_LOG_ERROR(omg_base, "Failed to load Raylib music ", path->ptr);
        return NULL;
    }
    return mus;
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
    base->mus_destroy = omg_audio_raylib_mus_destroy;
    base->mus_play = omg_audio_raylib_mus_play;
    base->mus_stop = omg_audio_raylib_mus_stop;
    base->mus_set_volume = omg_audio_raylib_mus_set_volume;
    OMG_END_POINTER_CAST();
    base->type = OMG_AUDIO_TYPE_RAYLIB;
    base->inited = true;
    return false;
}
#endif
