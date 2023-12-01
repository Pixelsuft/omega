#include <omega/omega.h>

OMG_Omega* omg_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    return NULL;
}

void omg_log_set_level(struct OMG_Omega* this, const int log_level) {
    this->log_level = log_level;
}

void omg_log_info_str(OMG_Omega* this, const OMG_String* data) {
    OMG_UNUSED(this, data);
}

bool omg_destroy(OMG_Omega* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_init(OMG_Omega* this) {
    this->unused = NULL;
    this->std = NULL;
    this->should_free_mem = false;
    this->log_info_str = omg_log_info_str;
    this->log_set_level = omg_log_set_level;
    this->destroy = omg_destroy;
    return false;
}
