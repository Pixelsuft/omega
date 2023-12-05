#include <omega/omega.h>

static OMG_Omega* omg_def_omega = NULL;

OMG_API OMG_Omega* omg_get_default_omega(void) {
    return omg_def_omega;
}

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
    omg_def_omega = NULL;
    return false;
}

bool omg_init(OMG_Omega* this) {
    this->type = OMG_OMEGA_TYPE_NONE;
    this->unused = NULL;
    this->std = NULL;
    this->should_free_mem = false;
    if (this->log_level == -1)
        this->log_level = OMG_LOG_MIN_LEVEL;
    if (this->log_level_omg == -1)
        this->log_level_omg = OMG_LOG_OMG_MIN_LEVEL;
    // TODO: probably also handle long functions
    if (OMG_ISNULL(this->log_info_str))
        this->log_info_str = this->log_warn_str = this->log_error_str = this->log_fatal_str = omg_log_info_str;
    this->log_set_level = omg_log_set_level;
    this->destroy = omg_destroy;
    omg_def_omega = this;
    return false;
}
