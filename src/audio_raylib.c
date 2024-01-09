#include <omega/audio_raylib.h>

#if OMG_SUPPORT_RAYLIB
#include <omega/omega.h>
#define base ((OMG_Audio*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_audio_raylib_destroy(OMG_AudioRaylib* this) {
    if (!base->inited)
        return false;
    this->raylib->CloseAudioDevice();
    return false;
}

bool omg_audio_raylib_init(OMG_AudioRaylib* this) {
    omg_audio_init(base);
    this->raylib->InitAudioDevice();
    if (!this->raylib->IsAudioDeviceReady()) {
        _OMG_LOG_ERROR(omg_base, "Failed to init Raylib audio device");
        return true;
    }
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_audio_raylib_destroy;
    OMG_END_POINTER_CAST();
    base->type = OMG_AUDIO_TYPE_RAYLIB;
    base->inited = true;
    return false;
}
#endif
