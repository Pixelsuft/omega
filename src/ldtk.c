#include <omega/ldtk.h>

#if OMG_SUPPORT_LDTK
#include <omega/renderer.h>

bool omg_ldtk_destroy(OMG_Ldtk* this) {
    if (OMG_ISNOTNULL(this->levels.data)) {
        for (size_t i = 0; i < this->levels.len; i++) {
            if (OMG_ISNOTNULL(this->levels.data[i].name.ptr)) {
                omg_string_destroy(&this->levels.data[i].name);
            }
            if (OMG_ISNOTNULL(this->levels.data[i].layers.data)) {
                for (size_t j = 0; j < this->levels.data[i].layers.len; j++) {
                    if (OMG_ISNOTNULL(this->levels.data[i].layers.data[j].entities.data)) {
                        for (size_t h = 0; h < this->levels.data[i].layers.data[j].entities.len; h++) {
                            OMG_LdtkEntity* ent = &this->levels.data[i].layers.data[j].entities.data[h];
                            for (size_t k = 0; k < ent->props.len; k++) {
                                // TODO: free
                            }
                            OMG_ARRAY_DESTROY(&ent->props);
                        }
                    }
                    omg_string_destroy(&this->levels.data[i].layers.data[j].name);
                    OMG_ARRAY_DESTROY(&this->levels.data[i].layers.data[j].entities);
                    OMG_ARRAY_DESTROY(&this->levels.data[i].layers.data[j].tiles);
                }
            }
            OMG_ARRAY_DESTROY(&this->levels.data[i].layers);
        }
    }
    if (OMG_ISNOTNULL(this->tilemaps.data)) {
        for (size_t i = 0; i < this->tilemaps.len; i++) {
            if (OMG_ISNOTNULL(this->tilemaps.data[i].path.ptr)) {
                omg_string_destroy(&this->tilemaps.data[i].path);
            }
            if (OMG_ISNOTNULL(this->tilemaps.data[i].name.ptr)) {
                omg_string_destroy(&this->tilemaps.data[i].name);
            }
        }
    }
    if (OMG_ISNOTNULL(this->entities.data)) {
        for (size_t i = 0; i < this->entities.len; i++) {
            if (OMG_ISNOTNULL(this->entities.data[i].props.data))
                for (size_t j = 0; j < this->entities.data[i].props.len; j++) {
                    omg_string_destroy(&this->entities.data[i].props.data[j].name);
                }
            OMG_ARRAY_DESTROY(&this->entities.data[i].props);
            if (OMG_ISNOTNULL(this->entities.data[i].name.ptr)) {
                omg_string_destroy(&this->entities.data[i].name);
            }
        }
    }
    OMG_ARRAY_DESTROY(&this->levels);
    OMG_ARRAY_DESTROY(&this->tilemaps);
    OMG_ARRAY_DESTROY(&this->entities);
    return false;
}

// TODO: use binary search for this functions
int omg_ldtk_index_from_level_name(OMG_Ldtk* this, const OMG_String* level_name) {
    if (OMG_ISNULL(this->levels.data))
        return -1;
    for (int i = 0; i < (int)this->levels.len; i++) {
        if (omg_string_equal(&this->levels.data[i].name, level_name))
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_layer_name(OMG_LdtkLevel* level, const OMG_String* layer_name) {
    if (OMG_ISNULL(level->layers.data))
        return -1;
    for (int i = 0; i < (int)level->layers.len; i++) {
        if (omg_string_equal(&level->layers.data[i].name, layer_name))
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_entity_name(OMG_Ldtk* this, const OMG_String* entity_name) {
    if (OMG_ISNULL(this->entities.data))
        return -1;
    for (int i = 0; i < (int)this->entities.len; i++) {
        if (omg_string_equal(&this->entities.data[i].name, entity_name))
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_tilemap_name(OMG_Ldtk* this, const OMG_String* tilemap_name) {
    if (OMG_ISNULL(this->tilemaps.data))
        return -1;
    for (int i = 0; i < (int)this->tilemaps.len; i++) {
        if (omg_string_equal(&this->tilemaps.data[i].name, tilemap_name))
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_prop_name(OMG_LdtkEntityDef* ent_def, const OMG_String* prop_name) {
    if (OMG_ISNULL(ent_def->props.data))
        return -1;
    for (int i = 0; i < (int)ent_def->props.len; i++) {
        if (omg_string_equal(&ent_def->props.data[i].name, prop_name))
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_level_id(OMG_Ldtk* this, int level_id) {
    if (OMG_ISNULL(this->levels.data))
        return -1;
    for (int i = 0; i < (int)this->levels.len; i++) {
        if (this->levels.data[i].id == level_id)
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_layer_id(OMG_LdtkLevel* level, int layer_id) {
    if (OMG_ISNULL(level->layers.data))
        return -1;
    for (int i = 0; i < (int)level->layers.len; i++) {
        if (level->layers.data[i].id == layer_id)
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_entity_id(OMG_Ldtk* this, int entity_id) {
    if (OMG_ISNULL(this->entities.data))
        return -1;
    for (int i = 0; i < (int)this->entities.len; i++) {
        if (this->entities.data[i].id == entity_id)
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_tilemap_id(OMG_Ldtk* this, int tilemap_id) {
    if (OMG_ISNULL(this->tilemaps.data))
        return -1;
    for (int i = 0; i < (int)this->tilemaps.len; i++) {
        if (this->tilemaps.data[i].id == tilemap_id)
            return i;
    }
    return -1;
}

int omg_ldtk_index_from_prop_id(OMG_LdtkEntityDef* ent_def, int prop_id) {
    if (OMG_ISNULL(ent_def->props.data))
        return -1;
    for (int i = 0; i < (int)ent_def->props.len; i++) {
        if (ent_def->props.data[i].id == prop_id)
            return i;
    }
    return -1;
}

bool omg_ldtk_init(OMG_Ldtk* this, void* omg, char* data, size_t data_len) {
    this->omg = omg;
    this->entities.data = NULL;
    this->tilemaps.data = NULL;
    this->levels.data = NULL;
    if (
        OMG_ARRAY_INIT(&this->entities, 0, sizeof(OMG_LdtkEntityDef) * 4) ||
        OMG_ARRAY_INIT(&this->tilemaps, 0, sizeof(OMG_LdtkTilemapDef) * 4) ||
        OMG_ARRAY_INIT(&this->levels, 0, sizeof(OMG_LdtkLevel) * 4)
    ) {
        _OMG_LOG_ERROR(this->omg, "Failed to init arrays");
        return true;
    }
    for (size_t temp_i = 0; temp_i < data_len; temp_i++) {
        size_t i = temp_i;
        while ((temp_i < data_len) && (data[temp_i] != '\n') && (data[temp_i] != '\0'))
            temp_i++;
        data[temp_i] = '\0';
        if ((data[i] == 'I') && (data[i + 1] == 'N') && (data[i + 2] == 'F') && (data[i + 3] == 'O')) {
            // Info
            int buf[6];
            if (this->omg->std->sscanf(
                &data[i], "INFO,%i,%i,(%i,%i,%i,%i)", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse info");
                return true;
            }
            this->world_grid_size.w = (float)buf[0];
            this->world_grid_size.h = (float)buf[1];
            this->bg_color.r = (omg_color_t)(buf[2]) * OMG_MAX_COLOR / (omg_color_t)255;
            this->bg_color.g = (omg_color_t)(buf[3]) * OMG_MAX_COLOR / (omg_color_t)255;
            this->bg_color.b = (omg_color_t)(buf[4]) * OMG_MAX_COLOR / (omg_color_t)255;
            this->bg_color.a = (omg_color_t)(buf[5]) * OMG_MAX_COLOR / (omg_color_t)255;
        }
        else if ((data[i] == 'E') && (data[i + 1] == 'O') && (data[i + 2] == 'F')) {
            // End of file
            data[temp_i] = '\n';
            break;
        }
        else if (data[i] == 'T') {
            // Tile definition
            if (OMG_ISNULL(this->levels.data) || OMG_ISNULL(this->levels.data[this->levels.len - 1].layers.data)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tile");
                omg_ldtk_destroy(this);
                return true;
            }
            OMG_LdtkLevel* lev = &this->levels.data[this->levels.len - 1];
            OMG_LdtkLayer* lay = &lev->layers.data[lev->layers.len - 1];
            if (lay->is_entity_layer) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tile");
                omg_ldtk_destroy(this);
                return true;
            }
            OMG_LdtkTile tile;
            int buf[7];
            if (this->omg->std->sscanf(
                &data[i], "T,%i,%i,%i,%i,%i,%i,%i", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5], &buf[6]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tile");
                omg_ldtk_destroy(this);
                return true;
            }
            tile.src.x = (float)buf[0];
            tile.src.y = (float)buf[1];
            tile.pos.x = (float)buf[2];
            tile.pos.y = (float)buf[3];
            tile.flip_x = (buf[4] == 1) || (buf[4] == 3);
            tile.flip_y = (buf[4] == 2) || (buf[4] == 3);
            tile.id = buf[5];
            tile.a = (float)buf[6];
            if (OMG_ARRAY_PUSH(&lay->tiles, tile)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tile");
                omg_ldtk_destroy(this);
                return true;
            }
        }
        else if (data[i] == 'E') {
            // Entity
            if (OMG_ISNULL(this->levels.data) || OMG_ISNULL(this->levels.data[this->levels.len - 1].layers.data)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity");
                omg_ldtk_destroy(this);
                return true;
            }
            OMG_LdtkLevel* lev = &this->levels.data[this->levels.len - 1];
            OMG_LdtkLayer* lay = &lev->layers.data[lev->layers.len - 1];
            if (!lay->is_entity_layer) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity");
                omg_ldtk_destroy(this);
                return true;
            }
            OMG_LdtkEntity ent;
            int buf[7];
            if (this->omg->std->sscanf(
                &data[i], "E,%i,%i,%i,%i,%i,%i,%i", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5], &buf[6]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity");
                omg_ldtk_destroy(this);
                return true;
            }
            ent.id = buf[0];
            ent.rect.x = (float)buf[1];
            ent.rect.y = (float)buf[2];
            ent.grid_pos.x = (float)buf[3];
            ent.grid_pos.y = (float)buf[4];
            ent.rect.w = (float)buf[5];
            ent.rect.h = (float)buf[6];
            ent.props.data = NULL;
            ent.props.len = 0;
            if (OMG_ARRAY_PUSH(&lay->entities, ent)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity");
                omg_ldtk_destroy(this);
                return true;
            }
        }
        else if ((data[i] == 'P') && (data[i + 1] == ',')) {
            // Entity prop
            if (OMG_ISNULL(this->levels.data) || OMG_ISNULL(this->levels.data[this->levels.len - 1].layers.data)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse prop");
                omg_ldtk_destroy(this);
                return true;
            }
            OMG_LdtkLevel* lev = &this->levels.data[this->levels.len - 1];
            OMG_LdtkLayer* lay = &lev->layers.data[lev->layers.len - 1];
            int id, counter, type_val;
            if (OMG_ISNULL(lay->entities.data) || this->omg->std->sscanf(
                &data[i], "P,%i,%i,%i,", &id, &counter, &type_val
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse prop");
                omg_ldtk_destroy(this);
                return true;
            }
            OMG_LdtkEntity* ent = &lay->entities.data[lay->entities.len - 1];
            if (OMG_ISNULL(ent->props.data) && OMG_ARRAY_INIT(&ent->props, 0, sizeof(OMG_LdtkPropVal))) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse prop");
                omg_ldtk_destroy(this);
                return true;
            }
            bool res = false;
            OMG_LdtkPropVal val;
            if (type_val <= OMG_LDTK_BOOL) {
                int64_t buf = 0;
                res = this->omg->std->sscanf(&data[i], "P,%i,%i,%i,%lld", &id, &counter, &type_val, &buf) < 1;
                if (!res) {
                    if (type_val == OMG_LDTK_FLOAT) {
                        float f_val = (float)buf / 10000.0f;
                        this->omg->std->memcpy(&val, &f_val, sizeof(float));
                    }
                    else if (type_val == OMG_LDTK_BOOL) {
                        bool b_val = buf > 0;
                        this->omg->std->memcpy(&val, &b_val, sizeof(bool));
                    }
                    else {
                        int i_val = (int)buf;
                        this->omg->std->memcpy(&val, &i_val, sizeof(int));
                    }
                    res = OMG_ARRAY_PUSH(&ent->props, val);
                }
            }
            else if (type_val == OMG_LDTK_COLOR) {
                int r_buf, g_buf, b_buf, a_buf;
                res = this->omg->std->sscanf(&data[i], "P,%i,%i,%i,(%i,%i,%i,%i)", &id, &counter, &type_val, &r_buf, &g_buf, &b_buf, &a_buf) < 1;
                if (!res) {
                    OMG_Color col;
                    col.r = (omg_color_t)(r_buf) * OMG_MAX_COLOR / (omg_color_t)255;
                    col.g = (omg_color_t)(g_buf) * OMG_MAX_COLOR / (omg_color_t)255;
                    col.b = (omg_color_t)(b_buf) * OMG_MAX_COLOR / (omg_color_t)255;
                    col.a = (omg_color_t)(a_buf) * OMG_MAX_COLOR / (omg_color_t)255;
                    this->omg->std->memcpy(&val, &col, sizeof(OMG_Color));
                    res = OMG_ARRAY_PUSH(&ent->props, val);
                }
            }
            // TODO: string
            if (res) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse prop");
                omg_ldtk_destroy(this);
                return true;
            }
        }
        else if ((data[i] == 'L') && (data[i + 1] == 'E') && (data[i + 2] == 'V') && (data[i + 3] == 'E') && (data[i + 4] == 'L')) {
            // Level definition
            OMG_LdtkLevel lev;
            lev.layers.data = NULL;
            lev.name.type = OMG_STRING_NONE;
            if (this->omg->std->sscanf(
                &data[i], "LEVEL,%i,\"", &lev.id
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse level def");
                omg_ldtk_destroy(this);
                return true;
            }
            while (data[i] != '\"')
                i++;
            i++;
            size_t j = i;
            while (data[j] != '\"') {
                if (data[j] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse level def");
                    omg_ldtk_destroy(this);
                    return true;
                }
                j++;
            }
            int buf[9];
            if (this->omg->std->sscanf(
                &data[j], "\",%i,%i,%i,%i,%i,(%i,%i,%i,%i)", &buf[0], &buf[1], &buf[2], &buf[3],
                &buf[4], &buf[5], &buf[6], &buf[7], &buf[8]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse level def");
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '\0';
            OMG_String base_str = OMG_STRING_MAKE_STATIC(&data[i]);
            if (omg_string_init_dynamic(&lev.name, &base_str)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse level def");
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '\"';
            lev.depth = (float)buf[0];
            lev.rect.x = (float)buf[1];
            lev.rect.y = (float)buf[2];
            lev.rect.w = (float)buf[3];
            lev.rect.h = (float)buf[4];
            lev.bg_color.r = (omg_color_t)(buf[5]) * OMG_MAX_COLOR / (omg_color_t)255;
            lev.bg_color.g = (omg_color_t)(buf[6]) * OMG_MAX_COLOR / (omg_color_t)255;
            lev.bg_color.b = (omg_color_t)(buf[7]) * OMG_MAX_COLOR / (omg_color_t)255;
            lev.bg_color.a = (omg_color_t)(buf[8]) * OMG_MAX_COLOR / (omg_color_t)255;
            if (OMG_ARRAY_INIT(&lev.layers, 0, sizeof(OMG_LdtkLayer) * 4) || OMG_ARRAY_PUSH(&this->levels, lev)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse level def");
                OMG_ARRAY_DESTROY(&lev.layers);
                omg_string_destroy(&lev.name);
                omg_ldtk_destroy(this);
                return true;
            }
        }
        else if ((data[i] == 'L') && (data[i + 1] == 'A') && (data[i + 2] == 'Y') && (data[i + 3] == 'E') && (data[i + 4] == 'R')) {
            // Layer definition
            i += 7;
            if (OMG_ISNULL(this->levels.data) || (data[i - 1] != '\"')) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse layer def");
                omg_ldtk_destroy(this);
                return true;
            }
            OMG_LdtkLevel* lev = &this->levels.data[this->levels.len - 1];
            OMG_LdtkLayer lay;
            lay.entities.data = NULL;
            lay.tiles.data = NULL;
            lay.entities.len = 0;
            lay.tiles.len = 0;
            lay.name.type = OMG_STRING_NONE;
            size_t j = i;
            while (data[j] != '\"')
                j++;
            data[j] = '\0';
            OMG_String base_str = OMG_STRING_MAKE_STATIC(&data[i]);
            if (omg_string_init_dynamic(&lay.name, &base_str)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse layer def");
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '\"';
            int buf[12];
            if (this->omg->std->sscanf(
                &data[j], "\",%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4],
                &buf[5], &buf[6], &buf[7], &buf[8], &buf[9], &buf[10], &buf[11]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse level def");
                omg_ldtk_destroy(this);
                return true;
            }
            lay.level_id = buf[0];
            lay.id = buf[1];
            lay.tileset_id = buf[2];
            lay.is_entity_layer = buf[3] > 0;
            lay.size.w = (float)buf[4];
            lay.size.h = (float)buf[5];
            lay.grid_size = (float)buf[6];
            lay.total_offset.x = (float)buf[7];
            lay.total_offset.y = (float)buf[8];
            lay.offset.x = (float)buf[9];
            lay.offset.y = (float)buf[10];
            lay.visible = buf[11] > 0;
            if (
                lay.is_entity_layer ?
                OMG_ARRAY_INIT(&lay.entities, 0, 4 * sizeof(OMG_LdtkEntity)) :
                OMG_ARRAY_INIT(&lay.tiles, 0, 4 * sizeof(OMG_LdtkTile))
            ) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse layer def");
                omg_string_destroy(&lay.name);
                omg_ldtk_destroy(this);
                return true;
            }
            if (OMG_ARRAY_PUSH(&lev->layers, lay)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse layer def");
                omg_string_destroy(&lay.name);
                omg_ldtk_destroy(this);
                return true;
            }
        }
        else if (data[i] == 'F') {
            // Entity definition
            OMG_LdtkEntityDef ent;
            ent.name.type = OMG_STRING_NONE;
            if (this->omg->std->sscanf(
                &data[i], "F,%i,\"", &ent.id
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity def");
                omg_ldtk_destroy(this);
                return true;
            }
            while (data[i] != '\"')
                i++;
            i++;
            size_t j = i;
            while (data[j] != '\"') {
                if (data[j] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse entity def");
                    omg_ldtk_destroy(this);
                    return true;
                }
                j++;
            }
            int size_buf[2];
            if (this->omg->std->sscanf(
                &data[j], "\",%i,%i", &size_buf[0], &size_buf[1]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity def");
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '\0';
            OMG_String base_str = OMG_STRING_MAKE_STATIC(&data[i]);
            if (omg_string_init_dynamic(&ent.name, &base_str)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity def");
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '\"';
            ent.size.w = (float)size_buf[0];
            ent.size.h = (float)size_buf[1];
            ent.props.len = 0;
            ent.props.data = NULL;
            if (OMG_ARRAY_INIT(&ent.props, 0, sizeof(OMG_LdtkPropDef))) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity def");
                omg_string_destroy(&ent.name);
                omg_ldtk_destroy(this);
            }
            if (OMG_ARRAY_PUSH(&this->entities, ent)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity def");
                omg_string_destroy(&ent.name);
                omg_ldtk_destroy(this);
                return true;
            }
        }
        else if (data[i] == 'G') {
            // Entity prop definition
            OMG_LdtkPropDef prop;
            if (OMG_ISNULL(this->entities.data) || this->omg->std->sscanf(
                &data[i], "G,%i,%i,\"", &prop.id, &prop.val_type
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity prop def");
                omg_ldtk_destroy(this);
                return true;
            }
            while (data[i] != '\"')
                i++;
            i++;
            size_t j = i;
            while (data[j] != '\"') {
                if (data[j] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse tilemap def");
                    omg_ldtk_destroy(this);
                    return true;
                }
                j++;
            }
            data[j] = '\0';
            if (omg_string_init_dynamic(&prop.name, &OMG_STRING_MAKE_STATIC(&data[i]))) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity prop def");
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '"';
            if (OMG_ARRAY_PUSH(&this->entities.data[this->entities.len - 1].props, prop)) {
                omg_string_destroy(&prop.name);
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity prop def");
                omg_ldtk_destroy(this);
                return true;
            }
        }
        else if (data[i] == 'S') {
            // Tilemap definition
            OMG_LdtkTilemapDef tm;
            tm.name.type = OMG_STRING_NONE;
            if (this->omg->std->sscanf(
                &data[i], "S,%i,\"", &tm.id
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tilemap def");
                omg_ldtk_destroy(this);
                return true;
            }
            while (data[i] != '\"')
                i++;
            i++;
            size_t j = i;
            while (data[j] != '\"') {
                if (data[j] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse tilemap def");
                    omg_ldtk_destroy(this);
                    return true;
                }
                j++;
            }
            data[j] = '\0';
            OMG_String base_str = OMG_STRING_MAKE_STATIC(&data[i]);
            if (omg_string_init_dynamic(&tm.name, &base_str)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tilemap def");
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '\"';
            i = j = j + 3;
            while (data[j] != '\"') {
                if (data[j] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse tilemap def");
                    omg_string_destroy(&tm.name);
                    omg_ldtk_destroy(this);
                    return true;
                }
                j++;
            }
            int buf[5];
            if (this->omg->std->sscanf(
                &data[j], "\",%i,%i,%i,%i,%i", &buf[0], &buf[1], &buf[2], &buf[3], &buf[4]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tilemap def");
                omg_string_destroy(&tm.name);
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '\0';
            base_str = OMG_STRING_MAKE_STATIC(&data[i]);
            if (omg_string_init_dynamic(&tm.path, &base_str)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tilemap def");
                omg_string_destroy(&tm.name);
                omg_ldtk_destroy(this);
                return true;
            }
            data[j] = '\"';
            tm.size.w = (float)buf[0];
            tm.size.h = (float)buf[1];
            tm.grid_size = (float)buf[2];
            tm.spacing = (float)buf[3];
            tm.padding = (float)buf[4];
            if (OMG_ARRAY_PUSH(&this->tilemaps, tm)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse tilemap def");
                omg_string_destroy(&tm.name);
                omg_string_destroy(&tm.path);
                omg_ldtk_destroy(this);
                return true;
            }
        }
        data[temp_i] = '\n';
    }
    return false;
}
#endif
