#pragma once
#include <omega/config.h>

#if OMG_SUPPORT_LDTK
#include <omega/ostd.h>
#include <omega/omega.h>
#include <omega/array.h>

typedef struct {
    OMG_String name;
    OMG_FPoint size;
    int id;
} OMG_LdtkEntityDef;

typedef struct {
    OMG_Omega* omg;
    OMG_Array(OMG_LdtkEntityDef) entities;
    OMG_FPoint world_grid_size;
    OMG_Color bg_color;
} OMG_Ldtk;

OMG_API bool omg_ldtk_init(OMG_Ldtk* this, void* omg, char* data, size_t data_len);
OMG_API bool omg_ldtk_destroy(OMG_Ldtk* this);
#endif
