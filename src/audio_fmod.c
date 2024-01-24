#include <omega/audio_fmod.h>

#if OMG_SUPPORT_FMOD
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define snd_base ((OMG_Sound*)snd)
#define omg_base ((OMG_Omega*)base->omg)
#define HAS_ERROR(res) ((res) != FMOD_OK)
#define IS_PLAYING(data) (OMG_ISNOTNULL((data)->channel))

static FMOD_RESULT OMG_FMOD_STD_PREFIX (*FMOD_Channel_GetUserData_callback)(FMOD_CHANNEL*, void**) = NULL;

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
    if (IS_PLAYING(mus)) {
        int res;
        if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetVolume(mus->channel, volume))) {
            _OMG_LOG_WARN(omg_base, "Failed to set audio volume (", FMOD_ErrorString(res), ")");
            return true;
        }
    }
    return false;
}

FMOD_RESULT omg_audio_fmod_mus_callback(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void* commanddata1, void* commanddata2) {
    OMG_UNUSED(commanddata1, commanddata2);
    if ((callbacktype == FMOD_CHANNELCONTROL_CALLBACK_END) && (controltype == FMOD_CHANNELCONTROL_CHANNEL)) {
        if (OMG_ISNULL(FMOD_Channel_GetUserData_callback)) {
            return FMOD_ERR_INVALID_HANDLE;
        }
        int res;
        OMG_MusicFmod* mus = NULL;
        if (HAS_ERROR(res = FMOD_Channel_GetUserData_callback((FMOD_CHANNEL*)channelcontrol, (void**)&mus))) {
            return res;
        }
        if (OMG_ISNULL(mus)) {
            return FMOD_ERR_INVALID_HANDLE;
        }
        mus->channel = NULL;
    }
    return FMOD_OK;
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

bool omg_audio_fmod_mus_play(OMG_AudioFmod* this, OMG_MusicFmod* mus, int loops, double pos, double fade_in) {
    OMG_UNUSED(fade_in); // TODO
    if (OMG_ISNOTNULL(mus->channel))
        omg_audio_fmod_mus_stop(this, mus);
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_System_PlaySound(this->sys, mus->mus, NULL, 1, &mus->channel))) {
        mus->channel = NULL;
        _OMG_LOG_WARN(omg_base, "Failed to play audio (", FMOD_ErrorString(res), ")");
        return true;
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_SetLoopCount(mus->mus, loops))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio loop count (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetPosition(mus->channel, (int)(pos * 1000.0), FMOD_TIMEUNIT_MS))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio start pos (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetUserData(mus->channel, (void*)mus))) {
        _OMG_LOG_WARN(omg_base, "Failed to pass audio handle to channel data (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetVolume(mus->channel, mus->vol_cache))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio volume (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetCallback(mus->channel, (FMOD_CHANNELCONTROL_CALLBACK)omg_audio_fmod_mus_callback))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio channel end callback (", FMOD_ErrorString(res), ")");
    }
    if ((mus->pitch_cache != 1.0f) && HAS_ERROR(res = this->fmod.FMOD_Channel_SetPitch(mus->channel, mus->pitch_cache))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio pitch (", FMOD_ErrorString(res), ")");
    }
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetPaused(mus->channel, 0))) {
        _OMG_LOG_WARN(omg_base, "Failed to resume audio (", FMOD_ErrorString(res), ")");
    }
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

double omg_audio_fmod_mus_get_pos(OMG_AudioFmod* this, OMG_MusicFmod* mus) {
    if (!IS_PLAYING(mus))
        return 0.0;
    int res;
    unsigned int pos;
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_GetPosition(mus->channel, &pos, FMOD_TIMEUNIT_MS))) {
        _OMG_LOG_WARN(omg_base, "Failed to get audio position (", FMOD_ErrorString(res), ")");
        return -1.0;
    }
    return (double)pos / 1000.0;
}

bool omg_audio_fmod_mus_set_pos(OMG_AudioFmod* this, OMG_MusicFmod* mus, double pos) {
    if (pos < 0.0)
        pos = 0.0;
    else if (pos > mus_base->duration) // Should I use this hack???
        pos = mus_base->duration - 0.001;
    if (!IS_PLAYING(mus))
        return false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetPosition(mus->channel, (unsigned int)(pos * 1000.0), FMOD_TIMEUNIT_MS))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio position (", FMOD_ErrorString(res), ")");
        return true;
    }
    return false;
}

bool omg_audio_fmod_mus_set_speed(OMG_AudioFmod* this, OMG_MusicFmod* mus, float speed) {
    mus->pitch_cache = speed;
    if (!IS_PLAYING(mus))
        return false;
    int res;
    if (HAS_ERROR(res = this->fmod.FMOD_Channel_SetPitch(mus->channel, speed))) {
        _OMG_LOG_WARN(omg_base, "Failed to set audio pitch (", FMOD_ErrorString(res), ")");
        return true;
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
        FMOD_LOOP_OFF | FMOD_2D,
        NULL,
        &mus->mus
    ))) {
        _OMG_LOG_ERROR(omg_base, "Failed to load music ", path->ptr, " (", FMOD_ErrorString(res), ")");
        omg_audio_mus_destroy(base, mus_base);
        return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->vol_cache = 1.0f;
    mus->pitch_cache = 1.0f;
    mus->channel = NULL;
    unsigned int len_buf;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_GetLength(mus->mus, &len_buf, FMOD_TIMEUNIT_MS))) {
        mus_base->duration = -1.0;
        _OMG_LOG_ERROR(omg_base, "Failed to get music length ", path->ptr, " (", FMOD_ErrorString(res), ")");
    }
    else
        mus_base->duration = (double)len_buf / 1000.0;
    return mus;
}

OMG_MusicFmod* omg_audio_fmod_snd_from_fp(OMG_AudioFmod* this, OMG_MusicFmod* mus, const OMG_String* path) {
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
    if (HAS_ERROR(res = this->fmod.FMOD_System_CreateSound(
        this->sys,
        path->ptr,
        FMOD_LOOP_OFF | FMOD_2D,
        NULL,
        &mus->mus
    ))) {
        _OMG_LOG_ERROR(omg_base, "Failed to load sound ", path->ptr, " (", FMOD_ErrorString(res), ")");
        omg_audio_mus_destroy(base, mus_base);
        return (OMG_MusicFmod*)omg_audio_dummy_mus_alloc(base, mus_base);
    }
    mus->vol_cache = 1.0f;
    mus->channel = NULL;
    unsigned int len_buf;
    if (HAS_ERROR(res = this->fmod.FMOD_Sound_GetLength(mus->mus, &len_buf, FMOD_TIMEUNIT_MS))) {
        mus_base->duration = -1.0;
        _OMG_LOG_ERROR(omg_base, "Failed to get sound length ", path->ptr, " (", FMOD_ErrorString(res), ")");
    }
    else
        mus_base->duration = (double)len_buf / 1000.0;
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
    FMOD_Channel_GetUserData_callback = this->fmod.FMOD_Channel_GetUserData;
    OMG_BEGIN_POINTER_CAST();
    base->update = omg_audio_fmod_update;
    base->destroy = omg_audio_fmod_destroy;
    base->mus_from_fp = omg_audio_fmod_mus_from_fp;
    base->mus_destroy = omg_audio_fmod_mus_destroy;
    base->mus_set_volume = omg_audio_fmod_mus_set_volume;
    base->mus_play = omg_audio_fmod_mus_play;
    base->mus_stop = omg_audio_fmod_mus_stop;
    base->mus_get_pos = omg_audio_fmod_mus_get_pos;
    base->mus_set_pos = omg_audio_fmod_mus_set_pos;
    base->mus_set_speed = omg_audio_fmod_mus_set_speed;
    // Hacky
    base->snd_from_fp = omg_audio_fmod_snd_from_fp;
    base->snd_destroy = omg_audio_fmod_mus_destroy;
    base->snd_set_volume = omg_audio_fmod_mus_set_volume;
    base->snd_play = omg_audio_fmod_mus_play;
    base->snd_stop = omg_audio_fmod_mus_stop;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
