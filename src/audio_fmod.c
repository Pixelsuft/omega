#include <omega/audio_fmod.h>

#if OMG_SUPPORT_FMOD
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define snd_base ((OMG_Sound*)snd)
#define omg_base ((OMG_Omega*)base->omg)
#define HAS_ERROR(res) ((res) != FMOD_OK)
#define IS_PLAYING(data) (OMG_ISNOTNULL((data)->channel))

bool omg_audio_fmod_update(OMG_AudioFmod* this) {
    int res = this->fmod.FMOD_System_Update(this->sys);
    if (HAS_ERROR(res)) {
        _OMG_LOG_WARN(omg_base, "Failed to update FMOD system (", FMOD_ErrorString(res), ")");
        return true;
    }
    return false;
}

bool omg_audio_fmod_destroy(OMG_AudioFmod* this) {
    if (!base->inited)
        return false;
    int retval = false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_Close(this->sys))) {
        _OMG_LOG_WARN(omg_base, "Failed to close FMOD system (", FMOD_ErrorString(res), ")");
        retval = true;
    }
    if (HAS_ERROR(res = this->fmod.FMOD_System_Release(this->sys))) {
        _OMG_LOG_WARN(omg_base, "Failed to release FMOD system (", FMOD_ErrorString(res), ")");
        retval = true;
    }
    retval = omg_fmod_dll_free(&this->fmod) || retval;
    return retval;
}

bool omg_audio_fmod_mus_set_volume(OMG_AudioFmod* this, OMG_MusicFmod* mus, float volume) {
    mus->vol_cache = volume;
    // TODO: error handling everywhere
    if (IS_PLAYING(mus)) {
        this->fmod.FMOD_Channel_SetVolume(mus->channel, volume);
    }
    return false;
}

bool omg_audio_fmod_mus_play(OMG_AudioFmod* this, OMG_MusicFmod* mus, int loops, double pos, double fade_in) {
    int res;
    // TODO: pause, after everything resume
    if (HAS_ERROR(res = this->fmod.FMOD_System_PlaySound(this->sys, mus->mus, NULL, 0, &mus->channel))) {
        mus->channel = NULL;
        _OMG_LOG_WARN(omg_base, "Failed to play audio (", FMOD_ErrorString(res), ")");
        return true;
    }
    this->fmod.FMOD_Channel_SetVolume(mus->channel, mus->vol_cache);
    return false;
}

bool omg_audio_fmod_mus_destroy(OMG_AudioFmod* this, OMG_MusicFmod* mus) {
    if (OMG_ISNULL(mus) || OMG_ISNULL(mus->mus))
        return false;
    mus->channel = NULL;
    bool retval = false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_Release(mus->mus))) {
        _OMG_LOG_ERROR(omg_base, "Failed to release audio (", FMOD_ErrorString(res), ")");
        retval = true;
    }
    omg_audio_mus_destroy(base, mus_base);
    return retval;
}

bool omg_audio_fmod_mus_stop(OMG_AudioFmod* this, OMG_MusicFmod* mus) {
    if (IS_PLAYING(mus)) {
        int res;
        if (HAS_ERROR(res = this->fmod.FMOD_Channel_Stop(mus->channel))) {
            _OMG_LOG_ERROR(omg_base, "Failed to stop audio (", FMOD_ErrorString(res), ")");
            return true;
        }
    }
    return false;
}

OMG_MusicFmod* omg_audio_fmod_mus_from_fp(OMG_AudioFmod* this, OMG_MusicFmod* mus, const OMG_String* path) {
    if (omg_string_ensure_null((OMG_String*)path))
        return NULL;
    if (OMG_ISNULL(mus)) {
        mus = OMG_MALLOC(omg_base->mem, sizeof(OMG_MusicFmod));
        if (OMG_ISNULL(mus))
            return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
        mus_base->was_allocated = true;
    }
    else
        mus_base->was_allocated = false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_CreateStream(
        this->sys,
        path->ptr,
        FMOD_LOOP_NORMAL | FMOD_2D,
        NULL,
        &mus->mus
    ))) {
        _OMG_LOG_ERROR(omg_base, "Failed to load music ", path->ptr, " (", FMOD_ErrorString(res), ")");
        omg_audio_mus_destroy(base, mus_base);
        return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->vol_cache = 1.0f;
    mus->channel = NULL;
    return mus;
}

bool omg_audio_fmod_init(OMG_AudioFmod* this) {
    omg_audio_init((OMG_Audio*)this);
    if (omg_fmod_dll_load(&this->fmod, omg_base->fmod_dll_path)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load FMOD dll");
        return true;
    }
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_Create(&this->sys, FMOD_VERSION))) {
        omg_fmod_dll_free(&this->fmod);
        return true;
    }
    if (HAS_ERROR(res = this->fmod.FMOD_System_Init(
        this->sys,
        OMG_MAX_PLAYING_MUSIC + OMG_MAX_PLAYING_SOUND,
        FMOD_INIT_NORMAL,
        NULL
    ))) {
        _OMG_LOG_WARN(omg_base, "Failed to init FMOD system (", FMOD_ErrorString(res), ")");
        this->fmod.FMOD_System_Close(this->sys);
        omg_fmod_dll_free(&this->fmod);
        return true;
    }
    OMG_BEGIN_POINTER_CAST();
    base->update = omg_audio_fmod_update;
    base->destroy = omg_audio_fmod_destroy;
    base->mus_from_fp = omg_audio_fmod_mus_from_fp;
    base->mus_destroy = omg_audio_fmod_mus_destroy;
    base->mus_set_volume = omg_audio_fmod_mus_set_volume;
    base->mus_play = omg_audio_fmod_mus_play;
    base->mus_stop = omg_audio_fmod_mus_stop;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
