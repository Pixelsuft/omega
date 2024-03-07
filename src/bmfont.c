#include <omega/bmfont.h>

#if OMG_SUPPORT_BMFONT
// TODO: make a lot of stuff

bool omg_bmfont_destroy(OMG_Bmfont* this) {
    this->page = NULL;
    this->ch_count = 0;
    OMG_ARRAY_DESTROY(&this->chars);
    return false;
}

bool omg_bmfont_calc_size(OMG_Bmfont* this, const OMG_String* text, OMG_FPoint* size_buf) {
    size_buf->x = 0.0f;
    size_buf->y = (float)this->line_height;
    for (size_t i = 0; i < text->len; i++) {
        size_t chr = (size_t)text->ptr[i]; // TODO: utf-8 support
        if ((chr > this->chars.len) || ((this->chars.data[chr].w <= 0) && (this->chars.data[chr].xa <= 0)))
            continue;
        size_buf->x += (float)this->chars.data[chr].xa;
    }
    return false;
}

bool omg_bmfont_render(OMG_Bmfont* this, const OMG_String* text, const OMG_FPoint* pos) {
    float cur_x = OMG_ISNULL(pos) ? 0.0f : pos->x;
    float cur_y = OMG_ISNULL(pos) ? 0.0f : pos->y;
    OMG_Bmchar* bm_char;
    bool res = false;
    OMG_FRect src_rect, dst_rect;
    dst_rect.w = dst_rect.h = 0.0f;
    for (size_t i = 0; i < text->len; i++) {
        size_t chr = (size_t)text->ptr[i]; // TODO: utf-8 support
        if ((chr > this->chars.len) || ((this->chars.data[chr].w <= 0) && (this->chars.data[chr].xa <= 0)))
            continue;
        bm_char = &this->chars.data[chr];
        src_rect.x = (float)bm_char->x;
        src_rect.y = (float)bm_char->y;
        src_rect.w = (float)bm_char->w;
        src_rect.h = (float)bm_char->h;
        dst_rect.x = cur_x + (float)bm_char->xo;
        dst_rect.y = cur_y + (float)bm_char->yo;
        dst_rect.w = (float)bm_char->w;
        dst_rect.h = (float)bm_char->h;
        res = this->ren->copy_ex(
            this->ren,
            this->page,
            &src_rect,
            &dst_rect,
            NULL, 0.0
        ) || res;
        cur_x += (float)bm_char->xa;
    }
    return res;
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
        size_t temp_id = i;
        data[i + cur_len] = '\0';
        if (data[i] == 'i') {
            // Info
            int bd = 0;
            int it = 0;
            int sm = 0;
            int aa = 0;
            while (data[i] != 's' || data[i + 1] != 'i' || data[i + 2] != 'z') {
                if (data[i + 2] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse info 1");
                    return true;
                }
                i++;
            }
            if (this->omg->std->sscanf(
                &data[i], "size=%i bold=%i italic=%i", &this->size, &bd, &it
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse info 2");
                return true;
            }
            while (data[i] != 's' || data[i + 1] != 'i' || data[i + 2] != 'z' || data[i + 3] != 'e') {
                if (data[i + 2] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse info 3");
                    return true;
                }
                i++;
            }
            while (data[i] != 's' || data[i + 1] != 'm' || data[i + 2] != 'o' || data[i + 3] != 'o' || data[i + 4] != 't') {
                if (data[i + 2] == '\0') {
                    _OMG_LOG_ERROR(this->omg, "Failed to parse info 4");
                    return true;
                }
                i++;
            }
            if (this->omg->std->sscanf(
                &data[i], "smooth=%i aa=%i padding=%i,%i,%i,%i spacing=%i,%i", &sm, &aa, &this->pad[0], &this->pad[1], &this->pad[2], &this->pad[3], &this->spac[0], &this->spac[1]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse info 5");
                return true;
            }
            this->bold = bd != 0;
            this->italic = it != 0;
            this->smooth = sm != 0;
            this->aa = aa != 0;
        }
        else if ((data[i] == 'c') && (data[i + 1] == 'o')) {
            // Common
            int tmp = 0;
            if (this->omg->std->sscanf(
                &data[i], "common lineHeight=%i base=%i scaleW=%i scaleH=%i pages=%i packed=%i", &this->line_height, &this->base, &this->scale_w, &this->scale_h, &tmp, &tmp
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to common bmfont info");
                return true;
            }
        }
        else if (data[i] == 'p') {
            // Page
        }
        else if ((data[i] == 'c') && (data[i + 4] == 's')) {
            // Chars
            int ch_count = 32;
            if (this->omg->std->sscanf(&data[i], "chars count=%i", &ch_count) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse chars");
                return true;
            }
            this->ch_count = (size_t)ch_count;
            // DO NOT TOUCH, it works somehow
            if (OMG_ARRAY_INIT(&this->chars, this->ch_count, sizeof(OMG_Bmchar))) {
                _OMG_LOG_ERROR(this->omg, "Failed to allocate chars array");
                return true;
            }
            this->omg->std->memset(this->chars.data, 0, this->chars.size);
        }
        else if (data[i] == 'c') {
            // Char
            if (OMG_ISNULL(this->chars.data)) {
                _OMG_LOG_ERROR(this->omg, "Chars array is not allocated");
                return true;
            }
            int buf[10];
            if (this->omg->std->sscanf(
                &data[i], "char id=%i   x=%i    y=%i     width=%i     height=%i    xoffset=%i     yoffset=%i     xadvance=%i     page=%i  chnl=%i",
                &buf[0], &buf[1], &buf[2], &buf[3], &buf[4], &buf[5], &buf[6], &buf[7], &buf[8], &buf[9]
            ) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse char");
                omg_bmfont_destroy(this);
                return true;
            }
            size_t prev_len = this->chars.len;
            if (prev_len <= (size_t)buf[0]) {
                if (OMG_ARRAY_SET_LEN(&this->chars, (size_t)(buf[0]), true)) {
                    _OMG_LOG_ERROR(this->omg, "Failed to realloc chars array");
                    omg_bmfont_destroy(this);
                    return true;
                }
            }
            size_t ch_id = (size_t)buf[0];
            this->chars.data[ch_id].x = buf[1];
            this->chars.data[ch_id].y = buf[2];
            this->chars.data[ch_id].w = buf[3];
            this->chars.data[ch_id].h = buf[4];
            this->chars.data[ch_id].xo = buf[5];
            this->chars.data[ch_id].yo = buf[6];
            this->chars.data[ch_id].xa = buf[7];
        }
        else if ((data[i] == 'k') && (data[i + 7] == 's')) {
            // Kernings
            int k_count = 32;
            if (this->omg->std->sscanf(&data[i], "kernings count=%i", &k_count) < 1) {
                _OMG_LOG_ERROR(this->omg, "Failed to parse kernings");
                omg_bmfont_destroy(this);
                return true;
            }
        }
        else if (data[i] == 'k') {
            // Kerning
        }
        data[temp_id + cur_len] = '\n';
        i = temp_id + cur_len + 1;
    }
    return false;
}
#endif
