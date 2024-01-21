#include <omega/font.h>

bool omg_fontmgr_destroy(OMG_FontMgr* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_fontmgr_init(OMG_FontMgr* this) {
    this->inited = false;
    this->destroy = omg_fontmgr_destroy;
    return false;
}
