#include <omega/ldtk.h>

#if OMG_SUPPORT_LDTK
// TODO: find functions

bool omg_ldtk_destroy(OMG_Ldtk* this) {
    if (OMG_ISNOTNULL(this->levels.data)) {
        for (size_t i = 0; i < this->levels.len; i++) {
            if (OMG_ISNOTNULL(this->levels.data[i].name.ptr)) {
                omg_string_destroy(&this->levels.data[i].name);
            }
            if (OMG_ISNOTNULL(this->levels.data[i].layers.data)) {
                for (size_t j = 0; j < this->levels.data[i].layers.len; j++) {
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
        else if ((data[i] == 'E') && (data[i] == 'O') && (data[i] == 'F')) {
            // End of file
            data[temp_i] = '\n';
            break;
        }
        else if (data[i] == 'T') {
            // Tile definition
        }
        else if (data[i] == 'E') {
            // Entity definition
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
            if (OMG_ISNULL(this->levels.data) || (data[i -1] != '\"')) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse layer def");
                omg_ldtk_destroy(this);
                return true;
            }
            OMG_LdtkLevel* lev = &this->levels.data[this->levels.len - 1];
            OMG_LdtkLayer lay;
            lay.entities.data = NULL;
            lay.tiles.data = NULL;
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
            if (OMG_ARRAY_PUSH(&this->entities, ent)) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse entity def");
                omg_string_destroy(&ent.name);
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
