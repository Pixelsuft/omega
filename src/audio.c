#include <omega/audio.h>

bool omg_audio_destroy(OMG_Audio* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_audio_init(OMG_Audio* this) {
    this->inited = false;
    this->type = OMG_AUDIO_TYPE_NONE;
    this->destroy = omg_audio_destroy;
    return false;
}
