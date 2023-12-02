#include <omega/omega.h>

OMG_Omega* omg_create(OMG_EntryData* data) {
    OMG_UNUSED(data);
    return NULL;
}

void omg_log_set_level(struct OMG_Omega* this, const int log_level, const int omg_log_level) {
    if (log_level >= 0)
        this->log_level = log_level;
    if (omg_log_level >= 0)
        this->log_level_omg = omg_log_level;
}

void omg_log_info_str(OMG_Omega* this, const OMG_String* data) {
    OMG_UNUSED(this, data);
}

bool omg_destroy(OMG_Omega* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_init(OMG_Omega* this) {
    this->type = OMG_OMG_TYPE_NONE;
    this->unused = NULL;
    this->std = NULL;
    this->should_free_mem = false;
    this->log_level = OMG_LOG_MIN_LEVEL;
    this->log_level_omg = OMG_LOG_OMG_MIN_LEVEL;
    this->log_info_str = this->log_warn_str = this->log_error_str = this->log_fatal_str = omg_log_info_str;
    this->log_set_level = omg_log_set_level;
    this->destroy = omg_destroy;
    return false;
}
