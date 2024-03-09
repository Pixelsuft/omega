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
    OMG_String name;
    OMG_String path;
    OMG_FPoint size;
    float grid_size;
    float spacing;
    float padding;
    int id;
} OMG_LdtkTilemapDef;

typedef struct {
    OMG_FRect rect;
    OMG_FRect grid_pos;
    int id;
} OMG_LdtkEntity;

typedef struct {
    OMG_FPoint src;
    OMG_FPoint pos;
    float a;
    bool flip_x;
    bool flip_y;
    int id;
} OMG_LdtkTile;

typedef struct {
    OMG_Array(OMG_LdtkEntity) entities;
    OMG_Array(OMG_LdtkTile) tiles;
    OMG_String name;
    OMG_FPoint total_offset;
    OMG_FPoint offset;
    OMG_FPoint size;
    float grid_size;
    int id;
    int level_id;
    int tileset_id;
    bool visible;
    bool is_entity_layer;
} OMG_LdtkLayer;

typedef struct {
    OMG_Array(OMG_LdtkLayer) layers;
    OMG_String name;
    OMG_FRect rect;
    OMG_Color bg_color;
    float depth;
    int id;
} OMG_LdtkLevel;

typedef struct {
    OMG_Omega* omg;
    OMG_Array(OMG_LdtkEntityDef) entities;
    OMG_Array(OMG_LdtkTilemapDef) tilemaps;
    OMG_Array(OMG_LdtkLevel) levels;
    OMG_FPoint world_grid_size;
    OMG_Color bg_color;
} OMG_Ldtk;

OMG_API bool omg_ldtk_init(OMG_Ldtk* this, void* omg, char* data, size_t data_len);
OMG_API bool omg_ldtk_destroy(OMG_Ldtk* this);
#endif
