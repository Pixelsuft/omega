#include <omega/ldtk.h>

#if OMG_SUPPORT_LDTK
bool omg_ldtk_destroy(OMG_Ldtk* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_ldtk_init(OMG_Ldtk* this, void* omg, char* data, size_t data_len) {
    OMG_UNUSED(data, data_len);
    this->omg = omg;
    return false;
}
#endif
