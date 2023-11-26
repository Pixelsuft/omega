#include <omega/omega.h>

OMG_Omega* omg_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    return NULL;
}

void omg_log_info_str(OMG_Omega* this, const char* data, size_t size) {
    OMG_UNUSED(this, data, size);
}

bool omg_destroy(OMG_Omega* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_init(OMG_Omega* this) {
    this->unused = NULL;
    this->should_free_mem = false;
    this->log_info_str = omg_log_info_str;
    this->destroy = omg_destroy;
    return false;
}
