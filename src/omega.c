#include <omega/omega.h>
#include <omega/omega_win.h>

OMG_Omega* omg_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    return NULL;
}

bool omg_destroy(OMG_Omega* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_init(OMG_Omega* this) {
    this->should_free_mem = false;
    this->destroy = omg_destroy;
    return false;
}
