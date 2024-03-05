#include <omega/bmfont.h>

#if OMG_SUPPORT_BMFONT
bool omg_bmfont_destroy(OMG_Bmfont* this) {
    this->page = NULL;
    this->ch_count = 0;
    return false;
}

bool omg_bmfont_init(OMG_Bmfont* this, OMG_Texture* page, OMG_Renderer* ren, char* data, size_t data_len) {
    this->ren = ren;
    this->omg = (OMG_Omega*)ren->omg;
    this->page = page;
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
            // Info
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
