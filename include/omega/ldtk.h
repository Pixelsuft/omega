#pragma once
#include <omega/config.h>

#if OMG_SUPPORT_LDTK
#include <omega/ostd.h>
#include <omega/omega.h>

typedef struct {
    OMG_Omega* omg;
    int dummy;
} OMG_Ldtk;

OMG_API bool omg_ldtk_init(OMG_Ldtk* this, void* omg, char* data, size_t data_len);
OMG_API bool omg_ldtk_destroy(OMG_Ldtk* this);
#endif
