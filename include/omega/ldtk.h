#pragma once
#include <omega/config.h>

#if OMG_SUPPORT_LDTK
#include <omega/ostd.h>
#include <omega/omega.h>
#include <omega/array.h>

#define OMG_LDTK_INT 1
#define OMG_LDTK_FLOAT 2
#define OMG_LDTK_BOOL 3
#define OMG_LDTK_STRING 4
#define OMG_LDTK_COLOR 5

typedef struct {
    OMG_String name;
    int val_type;
    int id;
} OMG_LdtkPropDef;

typedef struct {
    OMG_String name;
    OMG_FPoint size;
    OMG_Array(OMG_LdtkPropDef) props;
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
OMG_API int omg_ldtk_index_from_level_name(OMG_Ldtk* this, const OMG_String* level_name);
OMG_API int omg_ldtk_index_from_layer_name(OMG_LdtkLevel* level, const OMG_String* layer_name);
OMG_API int omg_ldtk_index_from_entity_name(OMG_Ldtk* this, const OMG_String* entity_name);
OMG_API int omg_ldtk_index_from_tilemap_name(OMG_Ldtk* this, const OMG_String* tilemap_name);
OMG_API int omg_ldtk_index_from_level_id(OMG_Ldtk* this, int level_id);
OMG_API int omg_ldtk_index_from_layer_id(OMG_LdtkLevel* level, int layer_id);
OMG_API int omg_ldtk_index_from_entity_id(OMG_Ldtk* this, int entity_id);
OMG_API int omg_ldtk_index_from_tilemap_id(OMG_Ldtk* this, int tilemap_id);
#endif
