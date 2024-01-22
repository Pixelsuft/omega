#include <omega/config.h>
#include <omega/font.h>

bool omg_fontmgr_destroy(OMG_FontMgr* this) {
    this->inited = false;
    return false;
}

OMG_Font* omg_fontmgr_dummy_font_create(OMG_FontMgr* this) {
    OMG_UNUSED(this);
#if OMG_ALLOW_DUMMY_FONT
    static OMG_Font font;
    font.size = 0.0f;
    font.aa = false;
    font.text_type = OMG_FONT_TEXT_TYPE_TEXT;
    return &font;
#else
    return NULL;
#endif
}

bool omg_fontmgr_font_destroy(OMG_FontMgr* this, OMG_Font* font) {
    OMG_UNUSED(this, font);
    return false;
}

OMG_Font* omg_fontmgr_font_from_fp(OMG_FontMgr* this, OMG_Font* font, const OMG_String* fp, long index, float size) {
    OMG_UNUSED(font, fp, index, size);
    return omg_fontmgr_dummy_font_create(this);
}

bool omg_fontmgr_font_set_scale(OMG_FontMgr* this, OMG_Font* font, const OMG_FPoint* scale) {
    OMG_UNUSED(this, font, scale);
    return false;
}

bool omg_fontmgr_font_query_text_size(OMG_FontMgr* this, OMG_Font* font, const OMG_String* text, OMG_FRect* size_buf) {
    OMG_UNUSED(this, font, text);
    size_buf->x = size_buf->y = 0.0f;
    size_buf->w = size_buf->h = 0.0f;
    return true;
}

bool omg_fontmgr_init(OMG_FontMgr* this) {
    this->inited = false;
    this->destroy = omg_fontmgr_destroy;
    this->font_from_fp = omg_fontmgr_font_from_fp;
    this->font_destroy = omg_fontmgr_font_destroy;
    this->font_set_scale = omg_fontmgr_font_set_scale;
    this->font_query_text_size = omg_fontmgr_font_query_text_size;
    return false;
}
