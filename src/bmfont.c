#include <omega/bmfont.h>

#if OMG_SUPPORT_BMFONT
bool omg_bmfont_destroy(OMG_Bmfont* this) {
    this->page = NULL;
    this->ch_count = 0;
    OMG_ARRAY_DESTROY(&this->chars);
    return false;
}

bool omg_bmfont_init(OMG_Bmfont* this, OMG_Texture* page, OMG_Renderer* ren, char* data, size_t data_len) {
    this->ren = ren;
    this->omg = (OMG_Omega*)ren->omg;
    this->page = page;
    this->chars.data = NULL;
    this->pad[0] = this->pad[1] = this->pad[2] = this->pad[3] = 0;
    this->spac[0] = this->spac[1] = 0;
    this->ch_count = 0;
    this->k_count = 0;
    this->size = 0;
    this->line_height = 0;
    this->base = 0;
    this->scale_w = 0;
    this->scale_h = 0;
    this->bold = false;
    this->italic = false;
    this->unicode = false;
    this->smooth = false;
    this->aa = false;
    // https://gist.github.com/viperscape/5812ee4cf63f02c1fcba8c5982295179
    // 13 = '\n'
    // 10 = '\0'
    for (size_t i = 0; i < data_len; ) {
        size_t cur_len = 0;
        while ((i + cur_len) < data_len && data[i + cur_len] != '\n') {
            cur_len++;
        }
        data[i + cur_len] = '\0';
        if (data[i] == 'i') {
            int bd = 0;
            int it = 0;
            int sm = 0;
            int aa = 0;
            while (data[i] != 's' || data[i + 1] != 'i' || data[i + 2] != 'z') {
                if (data[i + 2] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse info");
                    return true;
                }
                i++;
            }
            if (this->omg->std->sscanf(
                &data[i], "size=%i bold=%i italic=%i", &this->size, &bd, &it //, &sm, &aa, &this->pad[0], &this->pad[1], &this->pad[2], &this->pad[3], &this->spac[0], &this->spac[1]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse info");
                return true;
            }
            while (data[i] != 's' || data[i + 1] != 'i' || data[i + 2] != 'z' || data[i + 3] != 'e') {
                if (data[i + 2] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse info");
                    return true;
                }
                i++;
            }
            while (data[i] != 's' || data[i + 1] != 'm' || data[i + 2] != 'o' || data[i + 3] != 'o' || data[i + 4] != 't') {
                if (data[i + 2] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse info");
                    return true;
                }
                i++;
            }
            if (this->omg->std->sscanf(
                &data[i], "smooth=%i aa=%i padding=%i,%i,%i,%i spacing=%i,%i", &sm, &aa, &this->pad[0], &this->pad[1], &this->pad[2], &this->pad[3], &this->spac[0], &this->spac[1]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse info");
                return true;
            }
            this->bold = bd != 0;
            this->italic = it != 0;
            this->smooth = sm != 0;
            this->aa = aa != 0;
        }
        else if ((data[i] == 'c') && (data[i + 1] == 'o')) {
            // Common
        }
        else if (data[i] == 'p') {
            // Page
        }
        else if ((data[i] == 'c') && (data[i + 4] == 's')) {
            int ch_count = 32;
            if (this->omg->std->sscanf(&data[i], "chars count=%i", &ch_count) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse chars");
                return true;
            }
            this->ch_count = (size_t)ch_count;
            if (OMG_ARRAY_INIT(&this->chars, this->ch_count, 1)) {
                _OMG_LOG_ERROR(this->omg, "Failed to allocate chars array");
                return true;
            }
        }
        else if (data[i] == 'c') {
            // Char
        }
        else if ((data[i] == 'k') && (data[i + 7] == 's')) {
            // Kernings
        }
        else if (data[i] == 'k') {
            // Kerning
        }
        data[i + cur_len] = '\n';
        i += cur_len + 1;
    }
    return false;
}
#endif
