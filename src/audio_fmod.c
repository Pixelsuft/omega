#include <omega/audio_fmod.h>

#if OMG_SUPPORT_FMOD
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define mus_base ((OMG_Music*)mus)
#define snd_base ((OMG_Sound*)snd)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_audio_fmod_update(OMG_AudioFmod* this) {
    int res = this->fmod.FMOD_System_Update(this->sys);
    if (res != FMOD_OK) {
        _OMG_LOG_WARN(omg_base, "Failed to update FMOD system (", FMOD_ErrorString(res), ")");
        return true;
    }
    return false;
}

bool omg_audio_fmod_destroy(OMG_AudioFmod* this) {
    if (!base->inited)
        return false;
    omg_fmod_dll_free(&this->fmod);
    return false;
}

bool omg_audio_fmod_init(OMG_AudioFmod* this) {
    omg_audio_init((OMG_Audio*)this);
    if (omg_fmod_dll_load(&this->fmod, omg_base->fmod_dll_path)) {
        _OMG_LOG_ERROR(omg_base, "Failed to load FMOD dll");
        return true;
    }
    OMG_BEGIN_POINTER_CAST();
    base->update = omg_audio_fmod_update;
    base->destroy = omg_audio_fmod_destroy;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
