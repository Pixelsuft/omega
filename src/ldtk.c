#include <omega/ldtk.h>

#if OMG_SUPPORT_LDTK
bool omg_ldtk_destroy(OMG_Ldtk* this) {
    OMG_UNUSED(this);
    return false;
}

bool omg_ldtk_init(OMG_Ldtk* this, void* omg, char* data, size_t data_len) {
    this->omg = omg;
    for (size_t temp_i = 0; temp_i < data_len; temp_i++) {
        size_t i = temp_i;
        while ((temp_i < data_len) && (data[temp_i] != '\n') && (data[temp_i] != '\0'))
            temp_i++;
        data[temp_i] = '\0';
        if ((data[i] == 'I') && (data[i + 1] == 'N') && (data[i + 2] == 'F') && (data[i + 3] == 'O')) {
            // Info
        }
        else if ((data[i] == 'E') && (data[i] == 'O') && (data[i] == 'F')) {
            // End of file
            data[temp_i] = '\n';
            break;
        }
        else if (data[i] == 'F') {
            // Entity definition
        }
        else if (data[i] == 'S') {
            // Tilemap definition
        }
        else if (data[i] == 'T') {
            // Tile definition
        }
        else if (data[i] == 'E') {
            // Entity definition
        }
        else if ((data[i] == 'L') && (data[i + 1] == 'E') && (data[i + 2] == 'V') && (data[i + 3] == 'E') && (data[i + 4] == 'L')) {
            // Level definition
        }
        else if ((data[i] == 'L') && (data[i + 1] == 'A') && (data[i + 2] == 'Y') && (data[i + 3] == 'E') && (data[i + 4] == 'R')) {
            // Layer definition
        }
        data[temp_i] = '\n';
    }
    return false;
}
#endif
