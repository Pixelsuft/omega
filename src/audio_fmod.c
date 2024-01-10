#include <omega/audio_fmod.h>

#if OMG_SUPPORT_FMOD
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define snd_base ((OMG_Sound*)snd)
#define omg_base ((OMG_Omega*)base->omg)
#define HAS_ERROR(res) ((res) != FMOD_OK)

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
    OMG_END_POINTER_CAST();
    return false;
}
#endif
